---
name: soulslike-ue-production
description: 用于 UE5.8 魂类项目的规划、架构、多 Agent 协调、实现交接和生产流程决策。
metadata:
  short-description: 魂类 UE 生产流程
---

# 魂类 UE 生产流程

当任务跨越多个游戏系统，或需要决定负责人、阶段和验证方式时，使用本 Skill。

## 项目事实

- 引擎：Unreal Engine 5.8。
- 运行时模块：先读取项目现有模块边界，不假设固定模块名。
- 产品目标：单人魂类垂直切片。
- CLI、设计工作台、Agent 和 Skill 模板由外部 `SoulslikeForge` 套件提供。
- 游戏项目只保留 `.soulforge` 配置与设计数据、`.codex` 集成和游戏内容。
- C++ 负责稳定运行时系统，Blueprint 负责内容组合和表现。
- 首选基础：Gameplay Ability System、StateTree、Enhanced Input、CommonUI、Motion Warping、Control Rig 和 Niagara。
- Unreal 与源美术二进制资产必须使用 Git LFS。

## 工作流程

1. 检查项目并阅读 `AGENTS.md`。
2. 明确玩家可见结果和非目标。
3. 路由到最具体的全局 UE Skill，不能把本 Skill 当作领域技术指南的替代品。
4. 编码前明确权威归属、生命周期、数据所有权、存档或复制影响和所需资产。
5. 并行工作前定义互不冲突的文件所有权。
6. 一次完成一个垂直切片。
7. 编译、运行 PIE 或自动化、检查日志并报告证据。

## Agent 协调

- 主协调 Agent 负责范围、共享契约、`.Build.cs`、`.uproject`、集成和最终汇总。
- 最多同时使用三个专业 Agent。
- 只读调查和互不冲突的 C++ 工作可以并行。
- 地图、Blueprint、动画资产和 Blender 源文件必须通过单一编辑器会话串行修改。
- 禁止两个 Agent 同时修改同一个文件或同一类资产。

## 架构规则

- 运行时模块不得依赖编辑器模块。
- 没有明确边界需求前，不拆分模块，也不新增 Game Feature 插件。
- 可调战斗和敌人数值使用 DataAsset、DataTable、曲线或 Tag。
- 暴露给 Blueprint 的 UObject API 必须窄而且 GC 安全。
- 战斗时序必须可确定、可检查。

## 验证

使用 `$soulslike-verification`。优先执行针对性编辑器编译，收集 Blueprint 编译证据、命名明确的 PIE 场景和第一个可处理的 UE 日志错误。不能只凭源码检查报告完成。
