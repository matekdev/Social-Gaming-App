# Project Workflow Guide

### Project Organization
* Source (src) folders that contain cpp files must be named in PascalCase according to their primary class name.

### Git Workflow
* NEVER push directly into master. Always create a separate branch when making any change, no matter how small.
* Consistently rebase (effectively a merge) master branch into your working branch to reduce the size of merge conflicts. 
    * This is done with "git rebase master".
* NEVER delete any branches (as per the course rules).
* NEVER squash any commits (as per the course rules).

### Build
* ALWAYS build your project (using the README.md) in order to be able to merge your work in. Never break the build with your MR.
* Furthermore, it is advisable to run the project to test if any features have been impacted (ESPECIALLY if you are directly working on the feature).

### Code Review
* It is generally advisable to create a WIP ("Work-In-Progress") merge request when a branch is not completed, but still needs some review (or to show progress).
* Any MR that is not marked as WIP is assumed to be completed work (but not necessary code reviewed).
* Any MR MUST have at least 2 thumbs up from other teammates prior to being merged. The person who made the merge request must be responsible for merging the branch.

### Branch Naming
* Branches should be named in the following order <sfu id>/<branch type>/<branch name>
    * For example jdeans/feature/create-hello-world-class
    * Branch types are: 
        * feature - for developing user story features
        * testing - for writing/modifying test-specific code
        * refactor - for refactoring/reorganzation purposes
        * improvement - for minor improvements (i.e. UI improvement, minor features)

### C++/Code Standard
* The code specific guidelines are in the 'Coding Standards.md' file in the root directory.