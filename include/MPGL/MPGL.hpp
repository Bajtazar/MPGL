/**
 *  MPGL - Modern and Precise Graphics Library
 *
 *  Copyright (C) 2021-2022 Grzegorz Czarnecki
 *
 *  This software is provided 'as-is', without any express or implied
 *  warranty. In no event will the authors be held liable for any damages
 *  arising from the use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *  1. The origin of this software must not be misrepresented; you must not
 *     claim that you wrote the original software. If you use this software
 *     in a product, an acknowledgment in the product documentation would
 *     be appreciated but is not required.
 *
 *  2. Altered source versions must be plainly marked as such, and must not
 *     be misrepresented as being the original software.
 *
 *  3. This notice may not be removed or altered from any source
 *     distribution.
 */
#pragma once

#include <MPGL/Exceptions/ImageLoading/ImageLoadingFileCorruptionException.hpp>
#include <MPGL/Exceptions/ImageLoading/ImageLoadingInvalidTypeException.hpp>
#include <MPGL/Exceptions/Shader/ShaderLocationInvalidProgramException.hpp>
#include <MPGL/Exceptions/ImageLoading/ImageLoadingUnsuportedFileType.hpp>
#include <MPGL/Exceptions/Shader/ShaderLibraryInvalidShadersException.hpp>
#include <MPGL/Exceptions/ImageLoading/ImageLoadingFileOpenException.hpp>
#include <MPGL/Core/Vertex/Tessellation/CatmullClarkTessellator.hpp>
#include <MPGL/Exceptions/Shader/ShaderLocationUniformException.hpp>
#include <MPGL/Core/Vertex/Tessellation/SubdivisionTessellator.hpp>
#include <MPGL/Exceptions/Shader/ShaderProgramLinkingException.hpp>
#include <MPGL/Exceptions/Window/RenderWindowCompileException.hpp>
#include <MPGL/Exceptions/Window/WindowInvalidArgsException.hpp>
#include <MPGL/Exceptions/Shader/ShaderCompilationException.hpp>
#include <MPGL/Exceptions/TTFLoaderFileCorruptionException.hpp>
#include <MPGL/Exceptions/ExecusionUnknownPolicyException.hpp>
#include <MPGL/Mathematics/Transforms/ConvolutionKernels.hpp>
#include <MPGL/Utility/Deferred/DeferredExecutionWrapper.hpp>
#include <MPGL/Core/Transformations/ChainTransformation.hpp>
#include <MPGL/Core/Figures/Primitives/ConvexHexahedron.hpp>
#include <MPGL/Exceptions/Window/WindowGladException.hpp>
#include <MPGL/Exceptions/NotPerpendicularException.hpp>
#include <MPGL/Exceptions/DifferentPlanesException.hpp>
#include <MPGL/Core/Figures/Primitives/Tetrahedron.hpp>
#include <MPGL/Exceptions/FontNoRegularException.hpp>
#include <MPGL/Core/Transformations/Translation.hpp>
#include <MPGL/Core/Figures/Primitives/Cylinder.hpp>
#include <MPGL/Core/Figures/Primitives/Sphere.hpp>
#include <MPGL/Core/States/WiredFrameDisabler.hpp>
#include <MPGL/Core/Context/Buffers/BindGuard.hpp>
#include <MPGL/Core/States/WiredFrameEnabler.hpp>
#include <MPGL/Core/Transformations/Rotation.hpp>
#include <MPGL/Core/Windows/RenderingAdapter.hpp>
#include <MPGL/Core/Figures/Primitives/Torus.hpp>
#include <MPGL/Exceptions/StackedExceptions.hpp>
#include <MPGL/Core/Context/Buffers/Helpers.hpp>
#include <MPGL/Core/Transformations/Scaling.hpp>
#include <MPGL/Core/Figures/Primitives/Cone.hpp>
#include <MPGL/Core/Textures/TexturedFigure.hpp>
#include <MPGL/Mathematics/Transforms/IFCT.hpp>
#include <MPGL/IO/ImageLoading/ZigZacRange.hpp>
#include <MPGL/Core/Textures/TextureLoader.hpp>
#include <MPGL/Core/Layouts/AnchoredLayout.hpp>
#include <MPGL/IO/ImageLoading/JPEGLoader.hpp>
#include <MPGL/IO/ImageLoading/PNGLoader.hpp>
#include <MPGL/IO/ImageLoading/BMPLoader.hpp>
#include <MPGL/Core/Text/FontRasterizer.hpp>
#include <MPGL/Compression/ZlibDecoder.hpp>
#include <MPGL/Utility/StringAlgorithm.hpp>
#include <MPGL/Core/Windows/Window.hpp>
#include <MPGL/Core/Text/UTF-8.hpp>
#include <MPGL/Core/Text/Text.hpp>
