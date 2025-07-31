# VideoEditor Build and Deployment Instructions

## Prerequisites

### Development Environment
- **.NET 8.0 SDK** or later
- **Visual Studio 2022** (recommended) or **Visual Studio Code**
- **Windows 10/11** (for full Windows.Media API support)
- **Git** for version control

### For Windows Development (Full GUI Support)
```bash
# Install .NET 8.0 SDK
winget install Microsoft.DotNet.SDK.8

# Verify installation
dotnet --version
```

### For Cross-Platform Development (Console Demo)
The current implementation works on any platform with .NET 8.0 for the console demo.

## Building the Project

### Quick Start (Console Demo)
```bash
# Clone the repository
git clone <repository-url>
cd SpeechCore/VideoEditor

# Build the project
dotnet build

# Run the console demo
dotnet run
```

### Windows GUI Development Setup

#### 1. Update Project File for Windows
```xml
<Project Sdk="Microsoft.NET.Sdk">
  <PropertyGroup>
    <OutputType>WinExe</OutputType>
    <TargetFramework>net8.0-windows</TargetFramework>
    <UseWPF>true</UseWPF>
    <UseWindowsDesktop>true</UseWindowsDesktop>
    <Nullable>enable</Nullable>
  </PropertyGroup>
  
  <ItemGroup>
    <PackageReference Include="Microsoft.WindowsAPICodePack-Shell" Version="1.1.0" />
    <PackageReference Include="NAudio" Version="2.2.1" />
    <PackageReference Include="NAudio.WinMM" Version="2.2.1" />
    <PackageReference Include="NAudio.Wasapi" Version="2.2.1" />
    <!-- Add other Windows-specific packages -->
  </ItemGroup>
</Project>
```

#### 2. Required NuGet Packages

For full functionality, install these packages:

```bash
# Core packages
dotnet add package Microsoft.Extensions.DependencyInjection
dotnet add package Microsoft.Extensions.Logging
dotnet add package Microsoft.Extensions.Logging.Console
dotnet add package Newtonsoft.Json
dotnet add package System.Reactive

# Media processing packages
dotnet add package NAudio
dotnet add package NAudio.Core
dotnet add package NAudio.WinMM
dotnet add package NAudio.Wasapi

# Windows-specific packages
dotnet add package Microsoft.WindowsAPICodePack-Shell
dotnet add package System.Drawing.Common

# MVVM and UI packages
dotnet add package CommunityToolkit.Mvvm
```

#### 3. Windows Media Foundation Setup

For video processing capabilities, you'll need:

1. **Windows 10 SDK** (included with Visual Studio)
2. **Media Foundation libraries** (system components)

Add to your project:
```xml
<ItemGroup>
  <Reference Include="Windows">
    <HintPath>C:\Program Files (x86)\Windows Kits\10\UnionMetadata\$(TargetPlatformVersion)\Windows.winmd</HintPath>
  </Reference>
  <Reference Include="Windows.Media">
    <HintPath>C:\Program Files (x86)\Windows Kits\10\References\$(TargetPlatformVersion)\Windows.Media.winmd</HintPath>
  </Reference>
</ItemGroup>
```

## Visual Studio Configuration

### 1. Create New Project
1. Open Visual Studio 2022
2. Create new project → "WPF Application" (.NET)
3. Target Framework: .NET 8.0
4. Replace generated files with the VideoEditor implementation

### 2. Project Properties Configuration
- **Application**: 
  - Target Framework: .NET 8.0
  - Output Type: Windows Application
- **Build**:
  - Platform Target: x64 (recommended for media processing)
  - Define DEBUG for development builds
- **Debug**:
  - Start Action: Start Project
  - Enable native code debugging for media troubleshooting

### 3. Package Manager Console Commands
```powershell
# Install all required packages at once
Install-Package Microsoft.Extensions.DependencyInjection
Install-Package Microsoft.Extensions.Logging
Install-Package Microsoft.Extensions.Logging.Console
Install-Package Newtonsoft.Json
Install-Package System.Reactive
Install-Package NAudio
Install-Package NAudio.Core
Install-Package NAudio.WinMM
Install-Package NAudio.Wasapi
Install-Package Microsoft.WindowsAPICodePack-Shell
Install-Package CommunityToolkit.Mvvm
```

## Manifest Configuration

### App.manifest (Required for Windows)
The project includes an `app.manifest` file for:
- DPI awareness
- Administrator privileges (if needed for media access)
- Windows version compatibility

Ensure this file is referenced in your project:
```xml
<PropertyGroup>
  <ApplicationManifest>app.manifest</ApplicationManifest>
</PropertyGroup>
```

## Building Different Configurations

### Debug Build
```bash
dotnet build --configuration Debug
```
- Full logging enabled
- Debug symbols included
- Console output enabled
- Performance optimizations disabled

### Release Build
```bash
dotnet build --configuration Release
```
- Optimized performance
- Minimal logging
- No debug symbols
- Production-ready

### Platform-Specific Builds
```bash
# Windows x64
dotnet build --runtime win-x64

# Windows ARM64
dotnet build --runtime win-arm64
```

## Testing the Build

### Console Demo Test
```bash
dotnet run
```
Expected output:
- Service initialization logs
- Project creation demonstration
- Timeline operations
- Media engine simulation
- Clean shutdown

### Windows GUI Test (When Implemented)
1. Build the project
2. Run the executable
3. Verify UI components load
4. Test basic operations:
   - Create new project
   - Import media files
   - Timeline interaction
   - Preview functionality

## Deployment

### Self-Contained Deployment
```bash
# Create self-contained executable
dotnet publish --configuration Release --runtime win-x64 --self-contained true

# Single file deployment
dotnet publish --configuration Release --runtime win-x64 --self-contained true /p:PublishSingleFile=true
```

### Framework-Dependent Deployment
```bash
dotnet publish --configuration Release --runtime win-x64 --self-contained false
```

## Troubleshooting

### Common Build Issues

#### Missing Windows Desktop SDK
**Error**: `Microsoft.NET.Sdk.WindowsDesktop targets not found`
**Solution**: Install Visual Studio with .NET desktop development workload

#### NAudio Platform Issues
**Error**: `Platform not supported` for NAudio
**Solution**: Ensure targeting Windows platform or use NAudio.Core only

#### Media Foundation Access
**Error**: `Access denied` for media files
**Solution**: 
1. Run as administrator
2. Update app.manifest with required capabilities
3. Check file permissions

### Performance Issues
- Use Release configuration for performance testing
- Enable hardware acceleration in project settings
- Profile memory usage during media operations
- Monitor thread usage in timeline operations

### Debugging Tips
1. Enable native code debugging for media issues
2. Use Visual Studio diagnostic tools
3. Check Event Viewer for system-level errors
4. Monitor file handles and memory usage

## IDE Integration

### Visual Studio Extensions (Recommended)
- **ReSharper** or **Visual Studio IntelliCode** for code analysis
- **XAML Styler** for UI code formatting
- **Git Extensions** for version control
- **NuGet Package Manager** for dependency management

### VS Code Setup (Alternative)
```json
// .vscode/launch.json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": ".NET Core Launch (console)",
      "type": "coreclr",
      "request": "launch",
      "preLaunchTask": "build",
      "program": "${workspaceFolder}/bin/Debug/net8.0/VideoEditor.dll",
      "args": [],
      "cwd": "${workspaceFolder}",
      "console": "internalConsole",
      "stopAtEntry": false
    }
  ]
}
```

This build configuration provides a solid foundation for developing the video editor with all necessary dependencies and platform-specific optimizations.