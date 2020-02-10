# Nuclei Collider Code Sample

This example is a rudimentary simulation of nuclei collision events.  The functionality does not extend to sub-hadronic level processes; though there is some code in this implementation that could be used in the development of this feature.  The events are generated via Monte-Carlo sampling; the nuclei are filled, impacted, and statistics are collected over the given event configuration.

This code is provided as a code sample, under a BSD 3-Clause license.  The reuse of this code as a whole is not recommended, as there are a number of methods included for demonstration purposes only, and a better implementation of these methods should be used in a proper code release.


## Compilation

The source can be compiled by running the command make all from the terminal in the code directory.

```make
make all
```

### Tests

Tests can be compiled and run by running make tests.

```make
make tests
```

### Cleanup

The build directory can be cleaned by running make clean.

```make
make clean
```

## Usage

### Basic Usage

The code can be executed by running Collider.out from the code directory.

```bash
./Collider.out
```
In the default configuration, 1000 Pb+Pb events are simulated and statistical output is written to output/output.dat.

These statistics encompass a histogram of the number of binary nucleon-nucleon collisions per event, the number of nucleon participants per event, and the total nucleonic overlapping area per event.

### Advanced Usage

The executable can read in settings from a settings file (default: settings/settings.dat) and can also be given parameters from the command line.  Values given from the command line will have precedence over values listed in an input settings file, and both have precedence over the default values for any parameters changed.

The values in setting/settings.dat can be directly changed as desired, with the descriptions of the various parameters below.  Please exercise caution with setting parameters, as while there are some checks in place to catch bad parameter sets, it is possible to set parameters to bad values and undefined behavior may result.

From the command line, parameters can be changed by appending each tag with a dash "-" then a space followed by the desired value.  Multiple settings can be set this way, ensuring there is a space between.

As an example, the below command will run 500 p+Pb events and write the output statistics to output/RESULTS.dat.

```bash
./Collider.out -NumE 500 -nucA 0 -nproA 1 -nneuA 0 -outfile output/RESULTS.dat
```

The various parameters that can be set are as follows:

#### NumE <val>

Sets the number of simulated events to <val>.  The default value for this is 1000.

#### nucA <val> AND nucB <val>

Sets the type of each nucleus (A or B) participating in the collision: the <val> should be set to either 0 for a nucleus composed of a single nucleon, 1 for a deuteron, or 2 for any nucleus composed of more a single proton and neutron.  Please note the the nuclear density function is sampled from a spherically symmetric non-modified Woods-Saxon distribution, and may not be physically suitable for lighter nuclei (A<40) or unstable nuclei (ex U-235).  The default value for both of these is val=2 (heavy nucleus).

#### nproA <val> AND nproB <val>
Sets the number of protons in each nucleus (A or B) to <val>.  This should be consistent with the above nuc* setting (eg. do not set a deuteron with 3 protons).  The default value for this is val=82 (for a lead nucleus).

#### nneuA <val> AND nneuB <val>
Sets the number of neutrons in each nucleus (A or B) to <val>.  Similarly to setting the number of protons, this setting should be consistent with the nucleus type.  The default value for this is val=126 (for a Pb-208 nucleus).

#### binfilen <val> AND binfilea <val>
Sets the filename of the file containing bin ends used for histograms of event statistics.  The binfilen <val> gives bins used for collecting the number of nucleon-nucleon collisions per event as well as the number of nucleon participants per event.  The binfilea <val> gives bins used in the calculation of the total nucleon-nucleon overlapping area per event.  The bins for both of these should be ordered from least at the top of the file, to the greatest at the bottom of the file.  Overflow bins may be included at your discretion; just give a very large negative value at the top of the file and a very large positive value at the bottom.  The default values for these are val=settings/binfile_n.dat (for binfilen) and val=settings/binfile_a.dat (for binfilea).

#### outfile <val>
Sets the filename of the output file where the event statistics are written to.  This is currently done by listing the midpoint of a bin followed by the number of entries in the bin.  The file is ordered from smallest bin at the top of the file to the largest bin at the bottom (similar to the setup for the read-in binfile).  All three histograms are placed into a single file, each one preceded with a note of which histogram is below.  The default value for this is val=output/output.dat.

#### setfile <val>
Set the filename of the settings file for the various parameters.  This cannot be set or read from the settings file itself, it can only be set from the command line when invoking the executable.  This allows for multiple instances of the executable to be run with differing parameter values.  The default for this is val=settings/settings.dat.

## License
This code is distributed under a BSD 3-Clause license.
[BSD 3-Clause](https://opensource.org/licenses/BSD-3-Clause)