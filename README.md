# CECTool
A Nintendo 3ds Streetpass Management Tool.

Information for initial implemenation of cecd:u service taken from [here](https://www.3dbrew.org/wiki/CECD_Services) and [here](https://gist.github.com/wwylele/29a8caa6f5e5a7d88a00bedae90472ed). Further information can be found in [CECDocs](https://github.com/NarcolepticK/CECDocs) (*WIP*)

Runs on the 3ds and in Citra.

A 3dsx release is now available if you just want to try it out without compiling.

## Cloning
`git clone --recursive https://github.com/NarcolepticK/CECTool.git`

If there is an error about missing submodule files,

`git submodule init`

`git submodule update --recursive`

## Importing and Exporting Streetpasses
Exported Streetpasses are located on the Sd card in **/3ds/CECTool/export/streetpasses**. That directory contains a folder for each messagebox, based on the programId of the app, and inside there lies the messages exported.

To import, copy the exported folder(s) into **/3ds/CECTool/import/streetpasses**. CECTool parses that directory for programIds and compares against the list of activated boxes, and displays only the available boxes to import. You then select which message you would like to import, and can do so up to the message limit for that box.

## Main Menu
- #### Create
    - Create from exported backup
- #### Delete
    - Delete a box
    - Delete all boxes
    - Delete a Streetpass Message *Unimplemented*
    - Delete all Streetpass Messages in a Box *Unimplemented*
- #### Export
    - Export a box
    - Export all boxes
    - Export a Streetpass Message *Unimplemented*
    - Export all Streetpass Messages
- #### Import
    - Import a Box *Unimplemented*
    - Import Streetpass Messages
- #### Open
    - View & Explore *Partial*
- #### Tests
    - *Partial*

### Notes
Issues and suggestions are welcome. There are a large number of titles to test, and my sample size is small.

Tested import streetpasses with smash bros in Citra (HLE & LLE), and it registered hits.
