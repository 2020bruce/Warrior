# 设计摘要

> 本文件由 SoulForge DocSync 自动生成，请勿手动编辑。

## 项目

- 名称：Warrior
- 标题：魂类战斗设计控制台
- 目标：打造公平、可读、以风险与回报为核心的魂类战斗体验。
- 范围：单人垂直切片：1 个回环区域、3 把武器、5 种敌人、1 个 Boss。

## 功能状态

- active：6
- planned：6

## 阶段

| 阶段 | 状态 | 进度 |
|---|---|---|
| 项目基础 | done | 100% |
| 核心战斗原型 | active | 25% |
| 敌人与 Boss | planned | 0% |
| 关卡闭环 | planned | 0% |
| 垂直切片 | planned | 0% |
| 正式生产 | planned | 0% |

## 系统

| 系统 | 状态 | 负责人 | 依赖 |
|---|---|---|---|
| 角色与移动 | active | combat-engineer | - |
| 输入与缓存 | active | combat-engineer | `player` |
| 战斗核心 | active | combat-engineer | `input`, `animation` |
| 动画与帧数据 | active | animation-ai-engineer | `player` |
| 敌人 AI | planned | animation-ai-engineer | `combat`, `animation` |
| 关卡与检查点 | planned | content-engineer | `player`, `combat` |
| HUD 与菜单 | planned | ui-engineer | `combat` |
| 音频反馈 | planned | audio-engineer | `combat`, `animation` |
| 存档与进度 | planned | systems-engineer | `world` |
