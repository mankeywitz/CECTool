# CECTool
A POC CEC (StreetPass) injection tool.

Information for implemenation of CECD:U service taken from [here](https://www.3dbrew.org/wiki/CECD_Services) and [here](https://gist.github.com/wwylele/29a8caa6f5e5a7d88a00bedae90472ed)

Works for many games; more testing needed to find incompatible ones

### Cloning
`git clone --recursive https://github.com/NarcolepticK/CECTool.git`

If already set up,

`git submodule init`

`git submodule update --recursive`

Main Menu:
- Create
  - Create from exported backup
- Delete
  - Delete a box
  - Delete all boxes
- Export
  - Export a box
  - Export all boxes.
  - Export all Streetpasses
- Import
  - Import Streetpasses
- Open
  - Partial view/explore
- Tests
  - Partial

Issues are welcome to help increase compatibility. There are a large number of titles to test, and my sample size is small.

Tested import streetpasses with smash bros, and it registered hits.