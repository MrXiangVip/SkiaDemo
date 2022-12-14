/*
 * Copyright 2016 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SKSL_UTIL
#define SKSL_UTIL

#include <cstdarg>
#include <memory>
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "SkSLString.h"
#include "SkSLStringStream.h"

#if !defined(SKSL_STANDALONE) && SK_SUPPORT_GPU
#include "GrContextOptions.h"
#include "GrShaderCaps.h"
#endif

#ifdef SKSL_STANDALONE
#if defined(_WIN32) || defined(__SYMBIAN32__)
#define SKSL_BUILD_FOR_WIN
#endif
#else
#ifdef SK_BUILD_FOR_WIN
#define SKSL_BUILD_FOR_WIN
#endif // SK_BUILD_FOR_WIN
#endif // SKSL_STANDALONE

class GrShaderCaps;

namespace SkSL {

#if defined(SKSL_STANDALONE) || !SK_SUPPORT_GPU

// we're being compiled standalone, so we don't have access to caps...
enum GrGLSLGeneration {
    k110_GrGLSLGeneration,
    k130_GrGLSLGeneration,
    k140_GrGLSLGeneration,
    k150_GrGLSLGeneration,
    k330_GrGLSLGeneration,
    k400_GrGLSLGeneration,
    k420_GrGLSLGeneration,
    k310es_GrGLSLGeneration,
    k320es_GrGLSLGeneration,
};

#define SKSL_CAPS_CLASS StandaloneShaderCaps
class StandaloneShaderCaps {
public:
    GrGLSLGeneration generation() const {
        return k400_GrGLSLGeneration;
    }

    bool canUseMinAndAbsTogether() const {
        return true;
    }

    bool mustForceNegatedAtanParamToFloat() const {
        return false;
    }

    bool shaderDerivativeSupport() const {
        return true;
    }

    bool usesPrecisionModifiers() const {
        return true;
    }

    bool mustDeclareFragmentShaderOutput() const {
        return true;
    }

    bool fbFetchSupport() const {
        return true;
    }

    bool fbFetchNeedsCustomOutput() const {
        return false;
    }

    bool dropsTileOnZeroDivide() const {
        return false;
    }

    bool flatInterpolationSupport() const {
        return true;
    }

    bool noperspectiveInterpolationSupport() const {
        return true;
    }

    bool multisampleInterpolationSupport() const {
        return true;
    }

    bool externalTextureSupport() const {
        return true;
    }

    bool imageLoadStoreSupport() const {
        return true;
    }

    bool mustDoOpBetweenFloorAndAbs() const {
        return false;
    }

    bool mustEnableAdvBlendEqs() const {
        return false;
    }

    bool mustEnableSpecificAdvBlendEqs() const {
        return false;
    }

    bool canUseAnyFunctionInShader() const {
        return false;
    }

    bool floatIs32Bits() const {
        return true;
    }

    bool integerSupport() const {
        return false;
    }

    const char* shaderDerivativeExtensionString() const {
        return nullptr;
    }

    const char* fragCoordConventionsExtensionString() const {
        return nullptr;
    }

    const char* imageLoadStoreExtensionString() const {
        return nullptr;
    }

    const char* geometryShaderExtensionString() const {
        return nullptr;
    }

    const char* gsInvocationsExtensionString() const {
        return nullptr;
    }

    const char* externalTextureExtensionString() const {
        return nullptr;
    }

    const char* secondExternalTextureExtensionString() const {
        return nullptr;
    }

    const char* versionDeclString() const {
        return "";
    }

    bool gsInvocationsSupport() const {
        return true;
    }

    bool canUseFractForNegativeValues() const {
        return true;
    }

    bool canUseFragCoord() const {
        return true;
    }

    bool incompleteShortIntPrecision() const {
        return false;
    }

    bool addAndTrueToLoopCondition() const {
        return false;
    }

    bool unfoldShortCircuitAsTernary() const {
        return false;
    }

    bool emulateAbsIntFunction() const {
        return false;
    }

    bool rewriteDoWhileLoops() const {
        return false;
    }

    bool removePowWithConstantExponent() const {
        return false;
    }

    const char* fbFetchColorName() const {
        return nullptr;
    }
};

extern StandaloneShaderCaps standaloneCaps;

#else

#define SKSL_CAPS_CLASS GrShaderCaps
// Various sets of caps for use in tests
class ShaderCapsFactory {
public:
    static sk_sp<GrShaderCaps> Default() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fShaderDerivativeSupport = true;
        return result;
    }

    static sk_sp<GrShaderCaps> Version450Core() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 450 core";
        return result;
    }

    static sk_sp<GrShaderCaps> Version110() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 110";
        result->fGLSLGeneration = GrGLSLGeneration::k110_GrGLSLGeneration;
        return result;
    }

    static sk_sp<GrShaderCaps> UsesPrecisionModifiers() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fUsesPrecisionModifiers = true;
        return result;
    }

    static sk_sp<GrShaderCaps> CannotUseMinAndAbsTogether() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fCanUseMinAndAbsTogether = false;
        return result;
    }

    static sk_sp<GrShaderCaps> CannotUseFractForNegativeValues() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fCanUseFractForNegativeValues = false;
        return result;
    }

    static sk_sp<GrShaderCaps> MustForceNegatedAtanParamToFloat() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fMustForceNegatedAtanParamToFloat = true;
        return result;
    }

    static sk_sp<GrShaderCaps> ShaderDerivativeExtensionString() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fShaderDerivativeSupport = true;
        result->fShaderDerivativeExtensionString = "GL_OES_standard_derivatives";
        result->fUsesPrecisionModifiers = true;
        return result;
    }

    static sk_sp<GrShaderCaps> FragCoordsOld() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 110";
        result->fGLSLGeneration = GrGLSLGeneration::k110_GrGLSLGeneration;
        result->fFragCoordConventionsExtensionString = "GL_ARB_fragment_coord_conventions";
        return result;
    }

    static sk_sp<GrShaderCaps> FragCoordsNew() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fFragCoordConventionsExtensionString = "GL_ARB_fragment_coord_conventions";
        return result;
    }

    static sk_sp<GrShaderCaps> GeometryShaderSupport() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fGeometryShaderSupport = true;
        result->fGSInvocationsSupport = true;
        return result;
    }

    static sk_sp<GrShaderCaps> NoGSInvocationsSupport() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fGeometryShaderSupport = true;
        result->fGSInvocationsSupport = false;
        return result;
    }

    static sk_sp<GrShaderCaps> GeometryShaderExtensionString() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 310es";
        result->fGeometryShaderSupport = true;
        result->fGeometryShaderExtensionString = "GL_EXT_geometry_shader";
        result->fGSInvocationsSupport = true;
        return result;
    }

    static sk_sp<GrShaderCaps> GSInvocationsExtensionString() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fGeometryShaderSupport = true;
        result->fGSInvocationsSupport = true;
        result->fGSInvocationsExtensionString = "GL_ARB_gpu_shader5";
        return result;
    }

    static sk_sp<GrShaderCaps> VariousCaps() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fExternalTextureSupport = true;
        result->fFBFetchSupport = false;
        result->fDropsTileOnZeroDivide = true;
        result->fCanUseAnyFunctionInShader = false;
        return result;
    }

    static sk_sp<GrShaderCaps> CannotUseFragCoord() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fCanUseFragCoord = false;
        return result;
    }

    static sk_sp<GrShaderCaps> IncompleteShortIntPrecision() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 310es";
        result->fUsesPrecisionModifiers = true;
        result->fIncompleteShortIntPrecision = true;
        return result;
    }

    static sk_sp<GrShaderCaps> AddAndTrueToLoopCondition() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fAddAndTrueToLoopCondition = true;
        return result;
    }

    static sk_sp<GrShaderCaps> UnfoldShortCircuitAsTernary() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fUnfoldShortCircuitAsTernary = true;
        return result;
    }

    static sk_sp<GrShaderCaps> EmulateAbsIntFunction() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fEmulateAbsIntFunction = true;
        return result;
    }

    static sk_sp<GrShaderCaps> RewriteDoWhileLoops() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fRewriteDoWhileLoops = true;
        return result;
    }

    static sk_sp<GrShaderCaps> RemovePowWithConstantExponent() {
        sk_sp<GrShaderCaps> result = sk_make_sp<GrShaderCaps>(GrContextOptions());
        result->fVersionDeclString = "#version 400";
        result->fRemovePowWithConstantExponent = true;
        return result;
    }
};
#endif

void write_stringstream(const StringStream& d, OutputStream& out);

#if _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN __attribute__((__noreturn__))
#endif

NORETURN void sksl_abort();

} // namespace

#ifdef SKSL_STANDALONE
#define SkASSERT(x)
#define SkAssertResult(x) x
#define SkDEBUGCODE(x)
#endif

#define SKSL_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#if defined(__clang__) || defined(__GNUC__)
#define SKSL_PRINTF_LIKE(A, B) __attribute__((format(printf, (A), (B))))
#else
#define SKSL_PRINTF_LIKE(A, B)
#endif

#define ABORT(...) (printf(__VA_ARGS__), sksl_abort())

#endif
