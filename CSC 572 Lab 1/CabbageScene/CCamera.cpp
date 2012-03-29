#include "CCamera.h"

#include "../CabbageCore/glm/gtc/matrix_transform.hpp"

ICamera::ICamera()
    : LookDirection(0, 0, 1), Position(0, 0, 0)
{
    recalculateViewMatrix();
}

void ICamera::recalculateViewMatrix()
{
    //printf("Camera position: <%0.2f, %0.2f, %0.2f>; Camera lookat: <%0.2f, %0.2f, %0.2f>\n", Position.X, Position.Y, Position.Z, LookDirection.X, LookDirection.Y, LookDirection.Z);
    ViewMatrix = glm::lookAt(Position.getGLMVector(), (Position + LookDirection).getGLMVector(), glm::vec3(0, 1, 0));
}

SVector3 const & ICamera::getPosition() const
{
    return Position;
}

SVector3 const & ICamera::getLookDirecton() const
{
    return LookDirection;
}

void ICamera::setPosition(SVector3 const & position)
{
    Position = position;
}

void ICamera::setLookDirection(SVector3 const & lookDirection)
{
    LookDirection = lookDirection;
}

glm::mat4 const & ICamera::getViewMatrix() const
{
    return ViewMatrix;
}

glm::mat4 const & ICamera::getProjectionMatrix() const
{
    return ProjectionMatrix;
}


CPerspectiveCamera::CPerspectiveCamera(float const AspectRatio, float const Near, float const Far, float const FOV)
{
    setProjection(FOV, AspectRatio, Near, Far);
}

void CPerspectiveCamera::setProjection(float const FOV, float const AspectRatio, float const Near, float const Far)
{
    ProjectionMatrix = glm::perspective(FOV, AspectRatio, Near, Far);
}
