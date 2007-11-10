--
-- tolua++cegui premake script
--

cegui_dynamic("tolua++cegui", "c", "exe")

package.files =
{
	matchfiles(pkgdir.."*.c"),
	matchfiles(pkgdir.."*.h"),
}

-- We need to remove edit-and-continue in VS2003 (7.1) or older versions because 
-- of the __LINE__ expansion macro bug (see http://support.microsoft.com/kb/199057/en)
if target=="vs2002" or target=="vs2003" then
	package.buildflags = { "no-edit-and-continue" }
end

--include(pkgdir)
include(pkgdir.."../tolua++")

dependency("tolua++")
library("lua50","_d")
library_static("lua50","","_d")

if TOLUA_STATIC then
    define("TOLUA_STATIC")
end
