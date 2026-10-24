# Codex 注册表

> 本文件由 SoulForge DocSync 自动生成，请勿手动编辑。

## Agent

| Agent | 说明 | 文件 |
|---|---|---|
| `@animation-ai-engineer` | 负责魂类项目动画蓝图、Montage、动画通知、Motion Warping、StateTree、敌人 AI、感知和 Boss 行为资产。 | `.codex/agents/animation-ai-engineer.toml` |
| `@asset-optimizer` | 读取独立资产 QA 报告，在现有 Blender 源资产上定点修复漂浮、穿插、贴合、轮廓、比例和网格问题。 | `.codex/agents/asset-optimizer.toml` |
| `@asset-production-engineer` | 负责从 Studio 需求创建 Blender 资产、保存源文件、导出 FBX/GLB、生成预览并交付 Unreal 导入清单。 | `.codex/agents/asset-production-engineer.toml` |
| `@asset-quality-reviewer` | 独立检查 Blender 资产的技术完整性、多视角呈现和验收标准符合度，只输出结构化质量报告，不修改资产。 | `.codex/agents/asset-quality-reviewer.toml` |
| `@blender-asset-artist` | 负责 Blender 源资产、MCP 辅助建模与场景操作、重定向准备、LOD、碰撞命名和 Unreal 导入验证。 | `.codex/agents/blender-asset-artist.toml` |
| `@combat-engineer` | 负责魂类战斗 C++、能力、伤害流程、输入窗口、体力、削韧、锁定和战斗数据契约。 | `.codex/agents/combat-engineer.toml` |
| `@docs-maintainer` | 负责源码、配置、设计数据和 Codex 集成变化后的文档漂移检查、生成索引和人工设计文档同步。 | `.codex/agents/docs-maintainer.toml` |
| `@verifier` | 独立验证编译、Blueprint 与 StateTree 状态、PIE 场景、日志、资产、Git LFS 和性能证据。 | `.codex/agents/verifier.toml` |

## Skill

| Skill | 说明 | 文件 |
|---|---|---|
| `$asset-production` | 用于从 Studio 资产需求创建 Blender 模型、保存源文件、导出 FBX/预览并交付 Unreal 导入清单。 | `.codex/skills/asset-production/SKILL.md` |
| `$asset-quality-gate` | 用于 Blender 游戏资产的生产质量门禁、多视角评审、自动优化循环、最佳版本保留和 UE 导入前验证。 | `.codex/skills/asset-quality-gate/SKILL.md` |
| `$blender-asset-pipeline` | 用于 UE 魂类项目的 Blender 建模、MCP 场景自动化、重定向准备、网格与动画导出、LOD、碰撞命名和 Unreal 导入验证。 | `.codex/skills/blender-asset-pipeline/SKILL.md` |
| `$soulslike-combat` | 用于设计或实现魂类近战战斗、攻击帧数据、输入缓存、翻滚、体力、削韧、锁定、弹反、受击反应或 Boss 战。 | `.codex/skills/soulslike-combat/SKILL.md` |
| `$soulslike-docs-sync` | 用于文件变更后的文档漂移检查、生成文档重建、设计文档同步和 Git 提交门禁。 | `.codex/skills/soulslike-docs-sync/SKILL.md` |
| `$soulslike-ue-production` | 用于 UE5.8 魂类项目的规划、架构、多 Agent 协调、实现交接和生产流程决策。 | `.codex/skills/soulslike-ue-production/SKILL.md` |
| `$soulslike-verification` | 用于在报告完成前验证 UE5.8 魂类项目的编译、Blueprint 与 StateTree、PIE 行为、日志、资产、Git LFS 和性能。 | `.codex/skills/soulslike-verification/SKILL.md` |

## MCP

- `unreal-mcp`
- `blender`
