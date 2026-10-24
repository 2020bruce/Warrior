# Codex 工作流

## 套件入口

- 套件仓库：`D:\Program\SoulslikeForge`
- 游戏 CLI：全局 `soulforge`
- 本机项目配置：`.soulforge/config.json`
- 配置模板：`.soulforge/config.example.json`
- 设计数据：`.soulforge/design.json`
- 设计工作台：`soulforge studio`

## 项目 Skill

- `$soulslike-ue-production`
- `$soulslike-combat`
- `$blender-asset-pipeline`
- `$soulslike-verification`
- `$soulslike-docs-sync`

## 项目 Agent

- `@combat-engineer`：战斗运行时和战斗数据
- `@animation-ai-engineer`：动画、StateTree 和敌人 AI
- `@blender-asset-artist`：Blender 源资产与导入
- `@docs-maintainer`：文档漂移和生成索引
- `@verifier`：独立验证证据

## MCP

- Unreal MCP：`127.0.0.1:8000`
- Blender MCP：`localhost:9876`
- 本机 MCP 配置由 `soulforge mcp install --target project` 生成，不提交 Git。

## 固定命令

```powershell
soulforge doctor
soulforge build
soulforge test --suite Smoke
soulforge mcp status
soulforge blender
soulforge studio
soulforge docs status
soulforge docs build
soulforge docs check --staged
```

## 套件更新

- 默认更新模式：`notify`
- 只检查更新：`soulforge update check`
- 查看安装状态：`soulforge update status`
- 手动应用更新：`soulforge update apply`
- 验证安装文件：`soulforge update verify`
- 自动应用必须由用户显式执行 `soulforge update mode auto-apply`
- 安装版本记录在 `.soulforge/kit.lock.json`

## 并行与所有权

- 只读调查可以并行。
- C++ 文件必须按 Agent 所有权拆分。
- 并行 C++ 工作使用 `soulforge worktree create` 创建独立工作树。
- 地图、Blueprint、动画资产和 Blender 源文件串行修改。
- 共享接口、`.Build.cs` 和 `.uproject` 由主协调 Agent 负责。
- 文档生成索引由 DocSync 维护，人工设计文档由 `@docs-maintainer` 复核。
