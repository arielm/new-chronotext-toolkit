#pragma once

#include "cinder/gl/gl.h"
#include "cinder/Camera.h"

void perspective(float fovy, float aspect, float zNear, float zFar);
void perspective(float fovy, float zNear, float zFar, float width, float height, float panX = 0, float panY = 0, float zoom = 1);

void dumpCamera(const ci::Camera &cam, const std::string &name = "cam");
