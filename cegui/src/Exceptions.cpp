/***********************************************************************
    created:    20/2/2004
    author:     Paul D Turner, Frederico Jeronimo (fjeronimo)

    purpose:    Implements the exception classes used within the system
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
*
*   Permission is hereby granted, free of charge, to any person obtaining
*   a copy of this software and associated documentation files (the
*   "Software"), to deal in the Software without restriction, including
*   without limitation the rights to use, copy, modify, merge, publish,
*   distribute, sublicense, and/or sell copies of the Software, and to
*   permit persons to whom the Software is furnished to do so, subject to
*   the following conditions:
*
*   The above copyright notice and this permission notice shall be
*   included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
*   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
*   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
*   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
*   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
*   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
*   OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/PropertyHelper.h"
#include <iomanip>
#include <iostream>
#include <sstream>

#if defined( __WIN32__ ) || defined( _WIN32)
#   include <windows.h>
#elif defined(__ANDROID__)
#   include <android/log.h>
#endif

#if defined(_DEBUG) || defined(DEBUG)
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4091)
#   include <dbghelp.h>
#   pragma warning(pop)
#elif     (defined(__linux__) && !defined(__ANDROID__)) \
      ||  defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) \
      ||  defined(__HAIKU__)
#   include <execinfo.h>
#   include <dlfcn.h>
#   include <cxxabi.h>
#   include <cstddef>
#endif
#endif

namespace CEGUI
{
bool Exception::d_stdErrEnabled(true);

//----------------------------------------------------------------------------//
static void dumpBacktrace(size_t frames)
{
#if defined(_DEBUG) || defined(DEBUG)
#if defined(_MSC_VER)
    SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_INCLUDE_32BIT_MODULES);

    HANDLE process = GetCurrentProcess();

    if (!SymInitialize(process, nullptr, TRUE))
        return;

    HANDLE thread = GetCurrentThread();

    CONTEXT context;
    RtlCaptureContext(&context);

    STACKFRAME64 stackframe;
    ZeroMemory(&stackframe, sizeof(stackframe));
    stackframe.AddrPC.Mode = AddrModeFlat;
    stackframe.AddrStack.Mode = AddrModeFlat;
    stackframe.AddrFrame.Mode = AddrModeFlat;

#if _M_IX86
    stackframe.AddrPC.Offset = context.Eip;
    stackframe.AddrStack.Offset = context.Esp;
    stackframe.AddrFrame.Offset = context.Ebp;
    DWORD machine_arch = IMAGE_FILE_MACHINE_I386;
#elif _M_X64
    stackframe.AddrPC.Offset = context.Rip;
    stackframe.AddrStack.Offset = context.Rsp;
    stackframe.AddrFrame.Offset = context.Rbp;
    DWORD machine_arch = IMAGE_FILE_MACHINE_AMD64;
#endif

    char symbol_buffer[1024];
    ZeroMemory(symbol_buffer, sizeof(symbol_buffer));
    PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(symbol_buffer);
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol->MaxNameLen = sizeof(symbol_buffer) - sizeof(SYMBOL_INFO);

    Logger& logger(Logger::getSingleton());
    logger.logEvent("========== Start of Backtrace ==========", LoggingLevel::Error);

    size_t frame_no = 0;
    while (StackWalk64(machine_arch, process, thread, &stackframe,
                       &context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr) &&
           stackframe.AddrPC.Offset)
    {
        symbol->Address = stackframe.AddrPC.Offset;
        DWORD64 displacement = 0;

        std::stringstream sstream;
        sstream << "#" << frame_no << " ";

        if (SymFromAddr(GetCurrentProcess(), symbol->Address, &displacement, symbol))
        {
            char signature[256];
            UnDecorateSymbolName(symbol->Name, signature, sizeof(signature), UNDNAME_COMPLETE);
            sstream << signature;
        }
        else
        {
            const void* addressPtr = (void*)(ULONG_PTR)(symbol->Address);
            sstream << addressPtr;
        }
 
        IMAGEHLP_MODULE64 modinfo;
        modinfo.SizeOfStruct = sizeof(modinfo);

        const BOOL have_image_name =
            SymGetModuleInfo64(GetCurrentProcess(), symbol->Address, &modinfo);

        sstream << std::hex << std::setfill('0') << std::setw(16) << displacement << std::dec;
        sstream << " (" << have_image_name ? modinfo.LoadedImageName : "????";
        sstream << ")";

        logger.logEvent(CEGUI::String(sstream.str()), LoggingLevel::Error);

        if (++frame_no >= frames)
            break;

        if (!stackframe.AddrReturn.Offset)
            break;
    }

    logger.logEvent("==========  End of Backtrace  ==========", LoggingLevel::Error);

    SymCleanup(GetCurrentProcess());
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
    void* buffer[frames];
    const int received = backtrace(&buffer[0], frames);

    Logger& logger(Logger::getSingleton());

    logger.logEvent("========== Start of Backtrace ==========", LoggingLevel::Error);

    for (int i = 0; i < received; ++i)
    {
        std::string outstr;
        Dl_info info;
        if (dladdr(buffer[i], &info))
        {
            if (!info.dli_sname)
            {
                std::stringstream sstream;
                sstream << "#" << i << " " << static_cast<const void*>(buffer[i]) << " ("
                        << info.dli_fname << ")";
                outstr = sstream.str();
            }
            else
            {
                ptrdiff_t offset = static_cast<char*>(buffer[i]) -
                                   static_cast<char*>(info.dli_saddr);

                int demangle_result = 0;
                char* demangle_name = abi::__cxa_demangle(info.dli_sname, 0, 0, &demangle_result);

                std::stringstream sstream;
                sstream << "#" << i << " " << (demangle_name ? demangle_name : info.dli_sname) <<
                    " +" << std::hex << std::uppercase << std::showbase << offset << " (" <<
                    info.dli_fname << ")";
                outstr = sstream.str();

                std::free(demangle_name);
            }
        }
        else
        {
            std::stringstream sstream;
            sstream << "#" << i << " --- error ---";
            outstr = sstream.str();
        }

        logger.logEvent(outstr, LoggingLevel::Error);
    }

    logger.logEvent("==========  End of Backtrace  ==========", LoggingLevel::Error);

#else

    CEGUI_UNUSED(frames);
    
#endif

#else

    CEGUI_UNUSED(frames);

#endif
}

//----------------------------------------------------------------------------//
Exception::Exception(const String& message, const String& name,
                     const String& filename, int line, const String& function) :
    d_message(message),
    d_name(name),
    d_filename(filename),
    d_line(line),
    d_function(function),
    d_what(name + " in function '" + function +
           "' (" + filename + ":" + PropertyHelper<std::int32_t>::toString(line) + ") : " +
           message)
{
    // Log exception if possible
    if (Logger* const logger = Logger::getSingletonPtr())
    {
        logger->logEvent(d_what, LoggingLevel::Error);
        dumpBacktrace(64);
    }

    if (d_stdErrEnabled)
    {
        // output to stderr unless it's explicitly disabled
        // nobody seems to look in their log file!
        std::cerr << Exception::what() << std::endl;
    }
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "CEGUIBase", "Exception thrown: %s", what());
#endif
}

//----------------------------------------------------------------------------//
Exception::~Exception()  noexcept
{
}

//----------------------------------------------------------------------------//
const char* Exception::what() const noexcept
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    return d_what.c_str();
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    d_whatStdStr = String::convertUtf32ToUtf8(d_what.getString());
    return d_whatStdStr.c_str();
#endif
}

//----------------------------------------------------------------------------//
void Exception::setStdErrEnabled(bool enabled)
{
    d_stdErrEnabled = enabled;
}

//----------------------------------------------------------------------------//
bool Exception::isStdErrEnabled()
{
    return d_stdErrEnabled;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
