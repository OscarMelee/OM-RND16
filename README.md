# OM-RND16 - VCV Rack Random Voltage Module

A 16-channel random voltage generator module for VCV Rack 2.x.

## Features

- **16 Trigger Inputs**: Push signal inputs (left column) - rising edge triggers random voltage generation
- **16 Random Voltage Outputs**: Configurable voltage range outputs (right column)
- **Right-Click Menu**: Configure voltage range for each output individually
- **Compact 3HP Width**: Space-efficient design

## Installation

Download the latest `.vcvplugin` file from the [Releases](../../releases) page and copy it to your VCV Rack plugins folder:

- **Windows**: `Documents\Rack2\plugins-win-x64\`
- **Mac**: `~/Documents/Rack2/plugins-mac-x64/` (or `arm64` for M1/M2)
- **Linux**: `~/Documents/Rack2/plugins-lin-x64/`

Restart VCV Rack to load the new module.

## Usage

1. Connect trigger signals to the input jacks on the left
2. Connect the output jacks on the right to your destinations
3. Right-click the module to access voltage range settings
4. Each rising edge on an input generates a new random voltage on the corresponding output

## Voltage Ranges

- 0V to +10V (default)
- -10V to +10V
- -5V to +5V
- 0V to +5V
- 0V to +1V
- -1V to +1V
- 0V to +2V
- 0V to +3V
- 0V to +8V

## Author

OM-陈辰尘

## License

GPL-3.0-or-later
