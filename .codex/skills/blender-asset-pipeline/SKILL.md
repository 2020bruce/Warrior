---
name: blender-asset-pipeline
description: 用于 UE 魂类项目的 Blender 建模、MCP 场景自动化、重定向准备、网格与动画导出、LOD、碰撞命名和 Unreal 导入验证。
metadata:
  short-description: Blender 到 Unreal 资产管线
---

# Blender 资产管线

本 Skill 用于源美术和导出工作。游戏运行时仍由 Unreal Engine 负责。

## 工具

- Blender：`D:\Program\Blender\blender-4.5.15-windows-x64\blender.exe`
- Blender 插件：`MCP for Blender`，已在便携版偏好设置中启用。
- MCP 服务器：`blender`，配置位于 `.codex/config.toml`。
- Socket：`localhost:9876`。

## 安全要求

- 执行会修改场景的 MCP 操作前，先保存 `.blend`。
- 破坏性建模、减面、重拓扑或动画编辑优先使用副本。
- 没有版本控制时，不得覆盖已确认的源资产。
- 把 `execute_blender_code` 视为任意代码执行。一次只执行一个明确操作，并立即验证结果。
- 未经明确要求，不启用资产市场下载或生成服务。

## 源资产目录

使用稳定目录：

```text
SourceArt/
  Blender/
    Characters/
    Enemies/
    Weapons/
    Environment/
  Exports/
```

不要把源 `.blend` 文件放在 `Content/`。

## 建模与导出

- 场景单位使用米，并按真实比例建模。
- 除非既定绑定流程有要求，导出前应用旋转和缩放。
- 资产原点应位于预期的 Unreal 枢轴位置。
- 所有资产使用同一套经过验证的 FBX 或 glTF 导出预设。
- 只导出选中对象，并记录使用的预设。
- 尽可能保持一个源文件只包含一个角色或道具。
- 使用 Unreal 命名：`SM_`、`SK_`、`M_`、`MI_`、`T_`、`A_`、`AM_`、`ABP_`、`ST_`、`DT_`、`DA_`。
- 碰撞网格使用 `UCX_`、`UBX_` 或 `USP_` 前缀。
- 保持骨骼命名和重定向层级稳定。

## 验证

1. 从前、侧、后和四分之三视角检查资产。
2. 检查法线、切线、UV、材质槽、变换和原点。
3. 导入 Unreal 后验证缩放和朝向。
4. 检查骨骼层级、Socket、布料、物理资产和动画压缩。
5. 确认 LOD 和碰撞命名。
6. 在最终报告中记录 Unreal 资产验证结果或截图路径。

