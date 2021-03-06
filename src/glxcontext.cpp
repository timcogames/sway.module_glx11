#include <sway/glx11/glxcontext.h>

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <cassert>

NAMESPACE_BEGIN(sway)
NAMESPACE_BEGIN(glx11)

GlxContext::GlxContext(XScreenConnectionRef_t connection, XWindow * window)
	: _connection(connection)
	, _drawable(window->getWindowHandle()) {
	// Empty
}

GlxContext::~GlxContext() {
	if (!doneCurrent())
		printf("Failed to deactivate shared context before sharing.\n");

	glXDestroyContext(_connection->getDisplay(), _context);
}

void GlxContext::createLegacy(GLXFBConfig fbconfig) {
	_context = glXCreateNewContext(_connection->getDisplay(), fbconfig, GLX_RGBA_TYPE, 0, True);
	if (!_context)
		throw core::runtime::Exception("Couldn't create GLX context.");

	if (!glXIsDirect(_connection->getDisplay(), _context))
		printf("Unable to create direct rendering context.\n");
}

bool GlxContext::makeCurrent() {
	if (glXGetCurrentContext() != _context)
		return glXMakeCurrent(_connection->getDisplay(), _drawable, _context);

	return false;
}

bool GlxContext::doneCurrent() {
	return glXMakeCurrent(_connection->getDisplay(), None, NULL);
}

void GlxContext::present() {
	glXSwapBuffers(_connection->getDisplay(), _drawable);
}

NAMESPACE_END(glx11)
NAMESPACE_END(sway)
