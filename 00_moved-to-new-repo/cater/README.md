CATER for Atari and Apple2 Enhanced

This was ported by Oliver Schmidt, @Crawford adpated the fujinet-build-tools makefiles and hit some issues.

AndyDiller fixed this with addition of a local application.mk which passes in the required LDFlag for atari builds.

This now expects you to check out the repo: fujinet-build-tools in the same level directory as fujinet-apps.

It will then use the makefiles in that repo to build this project for you.
July 2024