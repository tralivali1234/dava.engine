CMAKE_FIND_APPBUNDLE
--------------------

This variable affects how ``find_*`` commands choose between
OS X Application Bundles and unix-style package components.

On Darwin or systems supporting OS X Application Bundles, the
``CMAKE_FIND_APPBUNDLE`` variable can be set to empty or
one of the following:

``FIRST``
  Try to find application bundles before standard programs.
  This is the default on Darwin.

``LAST``
  Try to find application bundles after standard programs.

``ONLY``
  Only try to find application bundles.

``NEVER``
  Never try to find application bundles.
