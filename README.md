# TaskManager

# 《个人任务管理工具》设计文档



## 1. 简介

该个人任务管理应用程序旨在帮助用户更好地管理任务和任务集。用户可以创建任务集，然后在每个任务集中添加、编辑和删除任务。应用程序提供直观的用户界面，方便用户快速有效地管理任务。

## 2. 功能特性

### 2.1 登录和注册

- 用户可以通过登录或注册来访问应用程序。
- 注册时，用户需要提供用户名、密码和电子邮件地址。
- 登录成功后，用户可以访问主界面。

### 2.2 主界面

- 主界面显示用户的任务集列表。
- 用户可以创建新的任务集、编辑现有的任务集或删除任务集。
- 点击任务集将显示任务列表。

### 2.3 任务集管理

- 用户可以为每个任务集设置名称和描述。
- 用户可以在任务集中添加、编辑和删除任务。
- 任务集中的任务可以按优先级、截止日期等排序。
- 用户可以标记任务为已完成或未完成。

### 2.4 任务管理

- 用户可以为每个任务设置名称、描述、优先级、截止日期等。
- 任务可以按照不同的状态进行筛选和排序。
- 用户可以标记任务为已完成或未完成。

### 2.5 数据同步

- 用户的任务数据将保存在数据库中，并在不同设备间同步。
- 用户在一个设备上的任务操作（创建、编辑、删除）将会同步到其他设备上。

## 3. 技术实现

### 3.1 编程语言、开发环境和平台

#### 3.1.1 编程语言

- 前端界面：使用 Qt 框架和 C++ 编程语言开发用户界面。
- 后端逻辑：使用 C++ 编程语言处理后端逻辑和数据库操作。

#### 3.1.2 开发环境

- 开发工具：Qt Creator 作为集成开发环境（IDE）。
- 编译工具：CMake 用于管理项目和构建。
- 版本控制：Git 用于版本控制和团队协作，Sourcetree 用于 Git 可视化操作管理。

#### 3.1.3 平台

- 目前项目基于Windows平台进行项目的构建、编译与运行
- 跨平台性：由于使用了 Qt 框架，以及使用了CMake进行项目构建，因此项目具有良好的跨平台性，可以在 Windows、Linux 和 macOS 等操作系统上使用CMake经过项目构建与编译后可以正常运行。
- 测试平台：在开发过程中，可以在各个目标平台上进行测试和调试，确保应用程序在不同环境下的稳定性和兼容性。

### 3.2 前端界面

- 使用 Qt 框架开发前端界面，包括登录界面、主界面、任务集管理界面和任务管理界面。
- 使用 Qt 的部件和布局管理器创建直观易用的用户界面。

### 3.3 后端逻辑

- 使用 C++ 编程语言编写后端逻辑，处理用户的请求和操作。
- 使用 SQLite 数据库存储用户的任务数据，包括用户、任务集和任务信息。
- 使用 Qt 的 SQL 模块与 SQLite 数据库进行交互。

### 3.4 数据库设计

#### 3.4.1 SQLite的优势

- 使用**关系数据库** **SQLite** 进行用户数据存储
- SQLite 的使用具有以下优点：

1. **轻量级**: SQLite 是一个嵌入式数据库，它的代码库非常小，不需要单独的服务器进程或配置，因此非常适合嵌入式设备和轻量级应用程序。
2. **无需配置**: SQLite 不需要复杂的配置过程，只需要一个文件就可以开始使用，这使得它非常易于部署和使用。
3. **跨平台**: SQLite 支持多种操作系统和编程语言，包括 Windows、Linux、macOS、iOS 和 Android 等，因此具有很好的跨平台性。
4. **开源免费**: SQLite 是开源软件，可以免费使用，而且它的源代码是公开的，用户可以根据需要进行修改和定制。

#### 3.4.2 数据库模型设计

- 全局E-R图如下：
  ![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image.png)

#### 3.4.3 面向对象设计

- 为实现用户、任务和任务集及其相关功能逻辑，基于面向对象设计的思想，设计了**任务**、**任务集**和**用户**3种C++类，用于实现所需各种功能：

![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image-7.png)

## 4. 项目文件结构和源代码简析

### 4.1 项目文件结构

<div style="display:flex;">
    <img src="image-8.png" alt="Image 1" style="width:50%;">
    <img src="image-9.png" alt="Image 2" style="width:50%;">
</div>

- 如上图所示，代码目录/src文件夹中包含了整个项目的C++代码文件（.h头文件和.cpp源文件）、CMake相关文件（CMakeLists.txt和CMakeLists.txt.user）、2个图片文件、若干个 .ui 文件（Qt Designer 创建的界面设计文件）以及src.qrc文件（Qt项目中资源文件的描述文件）

### 4.2 主要文件作用说明

- main.cpp 为C++程序的**入口**
- widget.h 及其源文件对应**主界面窗口**，widget.ui 文件为 Qt Designer 可视化界面设计文件：
  ![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image-10.png)
- login.h 及其源文件对应**登陆窗口**类，login.ui为其UI设计文件
  ![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image-11.png)
- register_w.h 及其源文件对应**注册窗口**类，register_w.ui为其UI设计文件
  ![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image-12.png)
- user.h及其源文件对应**用户类**
- taskset.h及其源文件对应**任务集类**
- task.h及其源文件对应**任务类**
- tcomboboxdelegate.h, tdatetimedelegate.h, ttextdelegate.h 及其源文件作为MVC模式中模型与视图之间的register_w，用于**编辑**视图中各项任务的数据
- dialogaddtask.h, dialogdeletetask.h, dialogdeletetaskset.h 及其源文件对应不同**对话框**类

## 5. 运行步骤指南

### 5.1 源代码编译运行

1. 可以在 IDE Qt Creator 中通过CMakeLists.txt文件打开src目录中的项目。
2. 使用 Qt Creator 重新编译项目并运行。
   ![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image-13.png)

### 5.2 运行已发布Windows程序

1. 本项目已通过Qt自带的Windows平台发布工具 windeployqt.exe进行打包。
2. 代码目录中的2个压缩包（task_manager_deploy - MinGW.zip和task_manager_deploy - MSVC.zip）即为使用2种编译器编译并打好的程序。
3. 可以直接将压缩文件解压，双击 task_manager_qt_v0.exe 直接运行。
   ![alt text](D:/Files/BaiduSyncdisk/%E5%A4%A7%E4%B8%89%E4%B8%8B/%E9%A1%B9%E7%9B%AE%E7%AE%A1%E7%90%86%E4%B8%8E%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1/%E5%B0%8F%E4%BD%9C%E4%B8%9A/%E6%8F%90%E4%BA%A4/%E6%96%87%E6%A1%A3%E7%9B%AE%E5%BD%95/image-15.png)

## 6. 优化和未来展望

- 相关组件的功能虽然实现，但界面仍有美化与优化的空间。
- 由于时间等因素，某些设计之初的期望功能并未能完全实现，如给任务加标签的功能，未来可以进一步完善。
- 目前项目只在Windows平台进行了编译运行，未来可以在Linux环境下进行重新编译运行。
- 可以进一步优化数据库访问和数据处理算法，提高应用程序的性能和响应速度。

