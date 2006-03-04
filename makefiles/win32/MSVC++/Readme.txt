So, what's going on here then?!  You're probably wondering what happened to the
projects for the other versions of Visual C++, so this note is here to explain
what we have done, and why we have done it.

Firstly, and probably most importantly, support for Visual C++ version 6 is now
discontinued.  We are aware that there are a large number of die-hard Visual C++
6 users out there who simply refuse to upgrade, well the fact is that Visual C++
6 was released in 1998 and, in software terms, is now ancient and totally
obsolete.  Add to this the fact that Microsoft Visual C++ 2005 Express edition
is available freely, and there really are no more excuses - so we have decided
that we can no longer devote resources to maintaining Visual C++ 6 compatibility
and project files.

Ok, so you're not a Visual C++ 6 or Visual C++ 7.0 user, and you're now either
wondering why we did away with the Visual C++ 7.1 projects, or when we'll
provide Visual C++ 8 projects...

Basically, maintaining multiple versions of the projects is a total nightmare,
and since each newer release of Visual C++ can import project files from
previous versions, the decision was taken to only provide project files for the
lowest supported version of Visual C++, at the moment, this is version 7.0
(a.k.a. Microsoft Visual C++ .Net 2002).  There are no plans to add project
files for other versions of Visual C++, the time and effort required to
constantly update and maintain multiple versions of what are essentially the
same thing, could be better spent elsewhere.

The CEGUI Team.