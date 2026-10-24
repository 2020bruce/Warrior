---
name: soulslike-docs-sync
description: 用于文件变更后的文档漂移检查、生成文档重建、设计文档同步和 Git 提交门禁。
metadata:
  short-description: 魂类项目文档同步
---

# 魂类项目文档同步

当源码、配置、Codex 集成、设计数据或资产变化后，使用本 Skill 更新相关文档。

## 工作流程

1. 运行 `soulforge docs status`。
2. 运行 `soulforge docs build` 更新事实型文档。
3. 读取 `Docs/DocDrift.md`，确认哪些设计文档需要复核。
4. 只修改被变更直接影响的人工文档。
5. 保留用户已有设计意图，不把待补充内容扩写成未经确认的新需求。
6. 运行 `soulforge docs check --all --strict`。
7. 对已经完成复核的文档运行 `soulforge docs acknowledge --doc <路径>`。

## 自动生成文档

以下文件由 DocSync 生成，不要手工维护：

- `Docs/Generated/SourceIndex.md`
- `Docs/Generated/CodexRegistry.md`
- `Docs/Generated/DesignSummary.md`
- `Docs/Generated/ProjectSettings.md`
- `Docs/Generated/AssetRegistry.md`
- `Docs/DocDrift.md`

## 人工维护文档

- `Docs/Architecture.md`
- `Docs/CombatDesign.md`
- `Docs/AssetPipeline.md`
- `Docs/CodexWorkflow.md`
- `Docs/DesignDecisions.md`

更新时要说明变更来源、行为影响、兼容性和未解决风险。不要把文档更新扩展到无关系统。

## 验证

- 生成文档与当前源码和配置一致。
- 机器状态写入 `Saved/SoulForgeDocSync/state.json`。
- 设计文档没有未解释的漂移。
- 暂存文件均能匹配 `.soulforge/docs-map.json`。
