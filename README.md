# CECTool
A Nintendo 3ds Streetpass Management Tool.

Information for initial implemenation of CECD:U service taken from [here](https://www.3dbrew.org/wiki/CECD_Services) and [here](https://gist.github.com/wwylele/29a8caa6f5e5a7d88a00bedae90472ed)

Works for many games; more testing needed to find incompatible ones

### Cloning
`git clone --recursive https://github.com/NarcolepticK/CECTool.git`

If there is an error about missing submodule files,

`git submodule init`

`git submodule update --recursive`

Main Menu:
- Create
  - Create from exported backup
- Delete
  - Delete a box
  - Delete all boxes
  - Delete a Streetpass Message *Unimplemented*
  - Delete all Streetpass Messages in a Box *Unimplemented*
- Export
  - Export a box
  - Export all boxes
  - Export a Streetpass Message *Unimplemented*
  - Export all Streetpass Messages
- Import
  - Import a Box *Unimplemented*
  - Import Streetpass Messages
- Open
  - View & Explore *Partial*
- Tests
  - *Partial*

Issues are welcome to help increase compatibility. There are a large number of titles to test, and my sample size is small.

Tested import streetpasses with smash bros, and it registered hits.
