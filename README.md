# ECE3-Superspeeder-Car
Using a TI development kit to build and program an autonomous car that uses IR sensor to follow a path in the shortest time possible

## Collaboration
We are going to be following [standard github flow](https://guides.github.com/introduction/flow/), basically meaning that we will be working in separate branhces, adding commits, then creating a pull request to main when our milestone feature is competed. The advantgae of this is that we can avoid merge conflicts, review each other's code and make suggestions, and potentially add the ability for testing if we decide it's valuable.

In this flow, once you clone the repo, we immedietaly want to change branches out of main.
```
git checkout -b my-branch 	// To create a new branch
git checkout my-branch		// If the branch already exists
```

## Timeline
- [x] Pins mapped and sensors calibrated using sensor fusion with differing weights
- [x] Code structure finalized and filled in. Deliverable is basic moving car of any form
- [ ] Optimize for robustness
- [ ] Optimize for speed
- [ ] Write report


11/10/2020 - commited initial working code that normalizes sensors, sums the data, responds based on the input and succesfully navigates the course to completion each time, pushed              to main branch under working_model_denver.ino
