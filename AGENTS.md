# Warrior 项目 Agent 规则

## 文档语言

- 项目内的设计文档、需求文档、流程文档、Skill、Agent 说明、测试报告和交接记录统一使用中文。
- 代码标识符、路径、类名、函数名、插件名、命令和第三方 API 名称保留英文原文。
- 新增文档默认使用中文，除非用户明确要求其他语言。

## 项目

- 引擎版本：Unreal Engine 5.8。
- 项目名：`Warrior`。
- 运行时模块：`Source/Warrior`。
- 当前目标：制作战斗手感优先的单人魂类垂直切片。
- Git 远程地址：`https://github.com/2020bruce/Warrior.git`。
- Unreal 二进制资产和 Blender 源资产使用 Git LFS 管理。

## 会话开始

1. 阅读 `.uproject`、`.codex/config.toml`、相关 `Source/` 与 `Config/` 文件，以及最接近的现有实现。
2. 如果存在 `Saved/CodexWorkflow/project-context.md`，先读取项目上下文。
3. 使用 `$soulslike-ue-production` 协调项目级工作。
4. 根据任务领域加载最具体的 UE Skill。
5. 在需求明确前保持只读调查，不直接修改文件。

## 架构

- 第一版生产内容继续放在现有 `Warrior` 运行时模块中。
- 可复用的战斗、伤害、存档、AI、输入和数据 API 优先使用 C++。
- 动画蓝图、视觉效果、控件、策划默认值和内容连接优先使用 Blueprint。
- 属性、效果、能力和伤害使用 Gameplay Ability System。
- 敌人和 Boss 决策使用 StateTree。
- 项目已启用 Enhanced Input、CommonUI、Motion Warping、Control Rig 和 Niagara。
- 没有明确需求前，不引入 Game Feature、Mass Entity、PCG 或 World Partition。
- 修改 `.Build.cs` 依赖或反射公共 API 前，必须检查其他 Agent 的文件所有权。

## 魂类战斗约束

- 输入缓存、后摇、无敌帧、体力、削韧、命中停顿、锁定和敌人读招都是核心系统。
- 战斗时序必须数据驱动，并以帧数或秒数配置，不能在代码中散落魔法数字。
- 默认只做单人游戏。加入多人前必须先重新设计权威、预测和复制方案。
- 战斗手感和公平可读性优先于内容数量。

## Blender 与资产管线

- Blender 4.5.15 LTS 便携版路径：`D:\Program\Blender\blender-4.5.15-windows-x64\blender.exe`。
- Blender MCP 已在 `.codex/config.toml` 中配置为 `blender` 服务器。
- 建模、导出、重定向、碰撞和 UE 导入使用 `$blender-asset-pipeline`。
- 任何 MCP 场景修改前，必须先保存 `.blend`。
- 没有版本控制或备份时，不得覆盖生产源资产。
- 在 Unreal 中验证缩放、朝向、枢轴、法线、材质槽、UV、碰撞和导入方向。

## 文件所有权

- 战斗工程师：`Source/Warrior/**/Combat`、战斗角色代码和战斗数据契约。
- 动画与 AI 工程师：动画蓝图、Montage、StateTree、AI 和敌人行为资产。
- Blender 资产美术：`SourceArt/Blender`、导出文件、重定向源和导入暂存区。
- 验证 Agent：编译、PIE、自动化、日志和性能证据，不修改生产内容。
- 主协调 Agent：共享接口、`.Build.cs`、`.uproject` 和跨 Agent 集成。

禁止两个 Agent 同时修改同一个文件、地图、Blueprint 或源 `.blend`。

## 生成目录

不得编辑或提交：

- `Binaries/`
- `Intermediate/`
- `DerivedDataCache/`
- `Saved/`
- `Build/`
- `.vs/`

`Saved/CodexWorkflow/` 可以保存本地工作流记录，但继续由 Git 忽略。

## 验证

- 修改 C++ 或模块后进行编辑器目标编译。
- 修改配置后验证 `.uproject`、`.uplugin`、JSON、TOML 和 Blueprint 编译状态。
- 在 PIE 或 Functional Test 中验证游戏行为。
- 修改运行时行为后读取 `Saved/Logs/Warrior.log` 或使用 UE MCP 日志工具。
- 添加二进制资产前必须确认 Git LFS 生效。
- 没有最新证据时，不得报告任务完成。

## 全局 CLI

确定性操作优先使用全局 CLI：

```powershell
soulforge doctor
soulforge build
soulforge editor
soulforge test --suite Smoke
soulforge mcp status
soulforge blender
soulforge studio
soulforge context
```

- 套件仓库：`D:\Program\SoulslikeForge`
- 套件远程地址：`https://github.com/2020bruce/SoulslikeForge`
- 本机路径和默认参数：`.soulforge\config.json`
- 可提交配置模板：`.soulforge\config.example.json`
- 游戏设计数据：`.soulforge\design.json`
- 修改引擎、Blender 或 uvx 路径时，优先修改 CLI 配置，不要在 Skill 和 Agent 中重复写死。
- Agent 调用 CLI 时使用 `--json` 获取结构化结果。
- CLI 不自动提交 Git、不删除资产、不自动打包。
- 可视化设计工作台由外部套件提供，游戏仓库只保留项目配置和设计数据。

## 文档同步

- 文件变化后运行 `soulforge docs status`。
- 更新生成型文档运行 `soulforge docs build`。
- 开发期间需要使用 `soulforge docs watch` 持续监听。
- 提交前运行 `soulforge docs check --staged`。
- 设计文档需要复核时读取 `Docs/DocDrift.md`，使用 `$soulslike-docs-sync`。
- `Docs/Generated/` 和 `Docs/DocDrift.md` 由 DocSync 自动生成，不得手工编辑。

## 套件更新

- 默认更新模式：`notify`。
- 检查更新：`soulforge update check`。
- 查看状态：`soulforge update status`。
- 手动应用：`soulforge update apply`。
- 验证安装：`soulforge update verify`。
- 只有用户明确要求时，才能切换到 `soulforge update mode auto-apply`。
- 自动更新不得覆盖 `.soulforge/config.json`、`.soulforge/design.json`、人工文档或游戏资产。

## 构建命令

```powershell
& "D:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" WarriorEditor Win64 Development -Project="D:\Program Files\Unreal Engine 5\Warrior\Warrior.uproject" -WaitMutex
```

```powershell
& "D:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe" "D:\Program Files\Unreal Engine 5\Warrior\Warrior.uproject"
```
