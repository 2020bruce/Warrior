# 架构

## 模块边界

- 运行时模块：`Warrior`
- 主要代码目录：`Source/Warrior/Public`、`Source/Warrior/Private`
- 游戏设计数据：`.soulforge/design.json`，包含阶段、阶段小任务、功能、系统和核心循环
- 资产设计目录：`.soulforge/assets.json`，记录现有资产用途、系统归属、标签和导入结果
- 制作套件：外部 `SoulslikeForge` 仓库
- 项目集成：`.soulforge`、`.codex/agents`、`.codex/skills`
- 本机路径：`.soulforge/config.json`，不进入 Git
- 可提交模板：`.soulforge/config.example.json`
- CLI：全局 `soulforge`，不在项目内保存启动器

## 系统所有权

- 战斗运行时由 C++ 提供稳定 API。
- 动画、视觉表现和内容连接使用 Blueprint。
- 属性和伤害使用 Gameplay Ability System。
- 敌人和 Boss 决策使用 StateTree。
- 输入使用 Enhanced Input。
- UI 使用 CommonUI。
- 资产源文件使用 Blender，并通过 Git LFS 管理。

## 输入与试玩

- 角色移动、镜头和跳跃由 `AWarriorHeroCharacter` 绑定到 `DefaultInput.ini` 的轴与动作映射。
- 锁定和无目标切换由 `AWarriorHeroController` 使用运行时 Enhanced Input 映射。
- `AWarriorBaseGameMode` 指定默认角色、PlayerController 和操作提示 HUD。
- Standalone 试玩使用 `UnrealEditor.exe -game`，不要求先生成安装包。
- 键鼠与手柄操作表通过 `.soulforge/config.json` 的 `playtest.controls` 配置，并显示在 Studio 验收区。

## 资产工作区

- Studio 通过文件系统扫描 `Content` 下的 `.uasset` 和 `.umap`，不要求 Unreal Editor 运行。
- 资产元数据独立保存在 `.soulforge/assets.json`，不修改 Unreal 资产本身。
- 新源文件先进入 `SourceArt/Imports/Incoming`，再由 UE5.8 Python commandlet 导入。
- 导入使用 `AssetImportTask`，并将脚本、日志和结果写入项目 `Saved` 目录。
- 删除暂存记录只会删除对应源文件，不删除已导入的 Unreal 资产。

## 已启用插件

- GameplayAbilities
- StateTree
- EnhancedInput
- CommonUI
- MotionWarping
- ControlRig
- Niagara
- EditorScriptingUtilities
- PythonScriptPlugin
- ModelContextProtocol
- EditorToolset

## 架构约束

- 运行时模块不得依赖编辑器模块。
- 可调战斗和敌人数值使用 DataAsset、DataTable、曲线或 Tag。
- 共享接口和 `.Build.cs` 由主协调 Agent 修改。
- 地图、Blueprint、动画和 Blender 源文件串行修改。
- Agent 并行修改 C++ 时使用独立 Git worktree 和互不重叠的写入范围。
- 没有明确需求前，不引入 Game Feature、Mass Entity、PCG 或 World Partition。

## 已知风险

- 当前项目仍处于核心战斗原型阶段，移动与镜头已在 Agent 工作树完成运行验证，等待用户验收。
- GAS、StateTree、CommonUI 等插件已启用，但玩法实现尚未展开。
- 网络和多人架构尚未确定，默认按单人游戏设计。

## 套件更新

- 套件源码位于独立 `SoulslikeForge` 仓库。
- 游戏项目通过 `.soulforge/kit.lock.json` 记录已安装版本和文件哈希。
- 默认更新模式为 `notify`，只检查并提示。
- `auto-apply` 必须由用户显式启用。
- 自动更新不得覆盖游戏配置、设计数据、用户修改的 Agent/Skill、人工文档或游戏资产。
- 用户修改过的管理文件会生成 `.new` 供人工合并。

## 版本控制

- `Content/`、`SourceArt/` 中的 Unreal、Blender、纹理和音频资产使用 Git LFS。
- `Config/`、`Source/`、`Docs/` 和设计 JSON 使用普通 Git 对象。
- `.codex/config.toml`、MCP 客户端配置和 `.soulforge/config.json` 属于本机环境，不提交。
- `.soulforge/design.backup.json` 由 Studio 保存时生成，不提交。
- `.sln`、`.slnx`、`Binaries/`、`Intermediate/`、`Saved/` 和 `.vs/` 不提交。
