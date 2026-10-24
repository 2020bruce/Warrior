---
name: asset-binding
description: 用于把 Studio 已生成、验证并导入 Unreal 的资产绑定到当前游戏功能工作树，同时保持输入、战斗时序、引用和验证边界。
metadata:
  short-description: Unreal 资产自动绑定
---

# Unreal 资产绑定

本 Skill 负责资产制作完成后的最后一公里，不负责重新生成资产。

## 输入

Studio 绑定任务必须提供：

- 功能 ID、功能标题和任务工作树。
- 资产槽位、资产名称、制作需求和绑定说明。
- 已导入的 `/Game/...` 资产路径。
- `asset-binding-result.json` 输出路径。

Studio 会把导入后的 `.uasset` 复制到任务工作树 `Content/`。绑定前先确认文件真实存在。

## 绑定原则

- 优先沿用现有组件、DataAsset、配置和加载方式。
- 保持既有输入、攻击窗口、碰撞、镜头和验收行为不变。
- 使用软引用或异步加载，避免无意义的硬引用扩大 cook 依赖。
- 一个资产需求只处理最小绑定点，不顺手重构无关代码。
- 资产路径、组件挂点、缩放、朝向和材质槽必须从实际资产验证。
- 不修改已生成的资产内容；需要重做时回到资产需求流程。

## 验证

1. 编译编辑器目标。
2. 运行针对性自动化测试。
3. 在任务专用验收场景中确认资产实际可见、可用。
4. 检查日志没有资产加载失败、空引用或 Blueprint 错误。
5. 写入结果文件：

```json
{
  "ok": true,
  "assetPaths": [
    "/Game/Assets/Weapons/Axe/SM_DefaultAxe.SM_DefaultAxe"
  ],
  "boundFiles": [
    "Source/Warrior/Private/Characters/WarriorHeroCharacter.cpp"
  ],
  "summary": "绑定到 WeaponMeshComponent；编译和输入级验收通过"
}
```

无法绑定时写 `ok=false`，并说明缺失依赖、阻塞位置和用户需要补充的信息。不得假装完成，不得执行 Git push。
