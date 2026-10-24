# 架构

## 模块边界

- 运行时模块：`Warrior`
- 主要代码目录：`Source/Warrior/Public`、`Source/Warrior/Private`
- 游戏设计数据：`.soulforge/design.json`
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

- 当前项目仍处于核心战斗原型阶段。
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
- `.sln`、`.slnx`、`Binaries/`、`Intermediate/`、`Saved/` 和 `.vs/` 不提交。
