ImRedraw
=====
This tiny header only library implements a first attempt to solve the Immediate Mode UI "problem" (Dear ImGui in this case) of having to redraw the UI every frame.
The idea is to analyze the draw list that gets created by Dear ImGui to detect changes that require the UI to be redrawn.
By analyzing the resulting vertex buffers instead of the application data there is no need for the user to clutter his codebase with intrusive code that tries to detect changes in data (or do so manually).

### Analyzing the vertex buffer
The most straightfoward approach that comes to mind is creating a copy of the whole draw list, store the previous frame in it and compare it vertex by vertex with the result of the next frame. However this doubles the necessary draw list memory (which depending on your use-case may or may not be okay). To avoid any allocations ImRedraw takes a different approach.
For a given frame ImRedraw calculates a SHA-1 hash from vertex position, color and UV data. By comparing the hash of the previous frame to the current frame, it is possible to determine whether or not a redraw is necessary.

### Pros and Cons of the hash approach
By calculating a hash of the vertex buffer almost no additional memory is necessary. The resulting hash is just 5 32-bit integers big (as defined for SHA-1).
One obvious drawback of this solution are hash collisions. Should two following frames erroneously produce the same hash value the UI will not be redrawn. I assume that the chances for this to happen are so slim as to be neglectable but someone with more hashing knowledge than me would have to confirm that. Depending on your use case this might be an issue and comparing the draw list vertex by vertex might be a better solution for you.

SHA-1: https://github.com/983/SHA1
