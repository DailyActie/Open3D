// ----------------------------------------------------------------------------
// -                        Open3D: www.open-3d.org                            -
// ----------------------------------------------------------------------------
// Imported on Apr 11th, 2018 from the Open3D project
// Copyright (c) 2018 www.open3d.org
// released under MIT licence
//
// A fork of their project is avilable at legacy-code.open-3d.org
// Refer to http://legacy-code.open-3d.org/blob/master/LICENSE
// for their full licence
// Check http://legacy-code.open-3d.org/graphs/contributors
// to get the list of their contributors and their contributions
//
// The rights of any later updates or modifications are reserved
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018, Hamdi Sahloul - www.open-3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <iostream>
#include <memory>
#include <thread>

#include <Open3D/Core/Core.h>
#include <Open3D/IO/IO.h>
#include <Open3D/Visualization/Visualization.h>

using namespace open3d;

class VisualizerWithDepthCapture : public VisualizerWithCustomAnimation
{
protected:
    void KeyPressCallback(GLFWwindow *window,
            int key, int scancode, int action, int mods) override {
        if (action == GLFW_RELEASE) {
            return;
        }
        if (key == GLFW_KEY_S) {
            CaptureDepthImage("depth.png");
            CaptureDepthPointCloud("depth.ply");
            PinholeCameraTrajectory camera;
            camera.extrinsic_.resize(1);
            view_control_ptr_->ConvertToPinholeCameraParameters(
                    camera.intrinsic_, camera.extrinsic_[0]);
            WriteIJsonConvertible("camera.json", camera);
        } else if (key == GLFW_KEY_L) {
            if (filesystem::FileExists("depth.png") &&
                    filesystem::FileExists("camera.json")) {
                PinholeCameraTrajectory camera;
                ReadIJsonConvertible("camera.json", camera);
                auto image_ptr = CreateImageFromFile("depth.png");
                auto pointcloud_ptr = CreatePointCloudFromDepthImage(
                        *image_ptr, camera.intrinsic_, camera.extrinsic_[0]);
                AddGeometry(pointcloud_ptr);
            }
        } else if (key == GLFW_KEY_K) {
            if (filesystem::FileExists("depth.ply")) {
                auto pointcloud_ptr = CreatePointCloudFromFile("depth.ply");
                AddGeometry(pointcloud_ptr);
            }
        } else if (key == GLFW_KEY_P) {
            if (filesystem::FileExists("depth.png") &&
                    filesystem::FileExists("camera.json")) {
                PinholeCameraTrajectory camera;
                ReadIJsonConvertible("camera.json", camera);
                view_control_ptr_->ConvertFromPinholeCameraParameters(
                        camera.intrinsic_, camera.extrinsic_[0]);
            }
        } else {
            VisualizerWithCustomAnimation::KeyPressCallback(
                    window, key, scancode, action, mods);
        }
        UpdateRender();
    }
};

int main(int argc, char *argv[])
{
    SetVerbosityLevel(VerbosityLevel::VerboseAlways);
    if (argc < 2) {
        PrintInfo("Usage:\n");
        PrintInfo("    > TestDepthCapture  [filename]\n");
        return 0;
    }

    auto mesh_ptr = CreateMeshFromFile(argv[1]);
    mesh_ptr->ComputeVertexNormals();
    PrintWarning("Press S to capture a depth image.\n");
    VisualizerWithDepthCapture visualizer;
    visualizer.CreateWindow("Depth Capture", 640, 480, 200, 200);
    visualizer.AddGeometry(mesh_ptr);
    visualizer.Run();
    visualizer.DestroyWindow();

    if (!filesystem::FileExists("depth.png") ||
            !filesystem::FileExists("camera.json")) {
        PrintWarning("Depth has not been captured.\n");
        return 0;
    }

    auto image_ptr = CreateImageFromFile("depth.png");
    DrawGeometries({image_ptr});

    PinholeCameraTrajectory camera;
    ReadIJsonConvertible("camera.json", camera);
    auto pointcloud_ptr = CreatePointCloudFromDepthImage(
            *image_ptr, camera.intrinsic_, camera.extrinsic_[0]);
    VisualizerWithDepthCapture visualizer1;
    visualizer1.CreateWindow("Depth Validation", 640, 480, 200, 200);
    visualizer1.AddGeometry(pointcloud_ptr);
    visualizer1.Run();
    visualizer1.DestroyWindow();

    PrintWarning("Press L to validate the depth image.\n");
    PrintWarning("Press P to load the capturing camera pose.\n");
    VisualizerWithDepthCapture visualizer2;
    visualizer2.CreateWindow("Depth Validation", 640, 480, 200, 200);
    visualizer2.AddGeometry(mesh_ptr);
    visualizer2.Run();
    visualizer2.DestroyWindow();

    PrintInfo("End of the test.\n");

    return 0;
}
