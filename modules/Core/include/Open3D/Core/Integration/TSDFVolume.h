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

#pragma once

#include <Open3D/Core/Geometry/RGBDImage.h>
#include <Open3D/Core/Geometry/PointCloud.h>
#include <Open3D/Core/Geometry/TriangleMesh.h>
#include <Open3D/Core/Camera/PinholeCameraIntrinsic.h>

namespace open3d {

/// Interface class of the Truncated Signed Distance Function (TSDF) volume
/// This volume is usually used to integrate surface data (e.g., a series of
/// RGB-D images) into a Mesh or PointCloud. The basic technique is presented in
/// the following paper:
/// B. Curless and M. Levoy
/// A volumetric method for building complex models from range images
/// In SIGGRAPH, 1996
class TSDFVolume
{
public:
    TSDFVolume(double voxel_length, double sdf_trunc, bool with_color) :
            voxel_length_(voxel_length), sdf_trunc_(sdf_trunc),
            with_color_(with_color) {}
    virtual ~TSDFVolume() {}

public:
    /// Function to reset the TSDFVolume
    virtual void Reset() = 0;

    /// Function to integrate an RGB-D image into the volume
    virtual void Integrate(const RGBDImage &image,
            const PinholeCameraIntrinsic &intrinsic,
            const Eigen::Matrix4d &extrinsic) = 0;

    /// Function to extract a point cloud with normals, using the marching cubes
    /// algorithm (https://en.wikipedia.org/wiki/Marching_cubes)
    virtual std::shared_ptr<PointCloud> ExtractPointCloud() = 0;

    /// Function to extract a triangle mesh, using the marching cubes algorithm
    /// (https://en.wikipedia.org/wiki/Marching_cubes)
    virtual std::shared_ptr<TriangleMesh> ExtractTriangleMesh() = 0;

public:
    double voxel_length_;
    double sdf_trunc_;
    bool with_color_;
};

}   // namespace open3d
