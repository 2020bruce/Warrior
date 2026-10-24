# Warrior

Warrior 是使用 Unreal Engine 5.8 开发的单人魂类游戏项目。
项目使用 [SoulslikeForge](https://github.com/2020bruce/SoulslikeForge)
作为外部制作套件，游戏仓库只保存源码、资产、项目配置和设计数据。

## 环境

- Unreal Engine `5.8`
- Visual Studio 2022，使用 `.vsconfig` 安装所需组件
- Git 和 Git LFS
- Blender `4.5.15 LTS`
- Node.js 24
- 全局 `soulforge` CLI
- Codex，Unreal MCP 与 Blender MCP

## 首次设置

```powershell
git lfs install
git lfs pull

git clone https://github.com/2020bruce/SoulslikeForge.git D:\Program\SoulslikeForge
powershell.exe -NoProfile -ExecutionPolicy Bypass `
  -File D:\Program\SoulslikeForge\scripts\install-cli.ps1

Copy-Item .soulforge\config.example.json .soulforge\config.json
```

检查并按本机路径修改 `.soulforge/config.json`，然后执行：

```powershell
soulforge doctor
soulforge mcp install --target project
soulforge mcp verify --target project
```

## 常用命令

```powershell
soulforge build
soulforge editor
soulforge test --suite Smoke
soulforge studio
soulforge context
soulforge docs status
```

所有命令都会自动发现当前 Unreal 项目，也可以显式传入：

```powershell
soulforge doctor --project-root "D:\Path\To\Warrior"
```

## 目录

```text
Config/                 UE 项目配置
Content/                UE 资产和关卡，使用 Git LFS
Docs/                   设计、架构和自动生成文档
Source/                 C++ 游戏代码
SourceArt/              Blender 源文件与导出目录
.codex/agents/          套件提供的 Agent
.codex/skills/          套件提供的项目 Skill
.soulforge/             设计数据、文档映射和套件锁
```

## 不提交内容

- `.soulforge/config.json`：本机引擎、Blender 和 uvx 路径
- `.codex/config.toml`、`.mcp.json`、`.cursor/`、`.gemini/`：本机 MCP 配置
- `.githooks/`：本机套件路径生成的提交钩子
- `Binaries/`、`Intermediate/`、`Saved/`、`Build/`、`.vs/`
- `.sln`、`.slnx` 和套件备份文件

这些文件可以通过 SoulslikeForge 安装器和 MCP 安装命令重新生成。

## 文档

- `AGENTS.md`：Agent 和开发流程规则
- `Docs/Architecture.md`：代码与模块架构
- `Docs/CombatDesign.md`：战斗设计
- `Docs/AssetPipeline.md`：Blender 与资产管线
- `Docs/DesignDecisions.md`：设计决策
- `Docs/Generated/`：DocSync 自动生成的事实索引
