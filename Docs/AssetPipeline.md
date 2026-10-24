# 资产管线

## 目录

- `SourceArt/Blender`：可编辑 Blender 源文件。
- `SourceArt/Exports`：发送到 Unreal 前的 FBX、GLB 或 USD 导出。
- `SourceArt/Imports/Incoming`：通过 Studio 上传的待导入源文件。
- `Content`：Unreal 资产和关卡。
- `.soulforge/assets.json`：现有资产的设计元数据和导入记录。

## 流程

1. 在 `SourceArt/Blender` 保存 `.blend`，确认 Git LFS 已跟踪。
2. 设置正确的单位、缩放、朝向和枢轴。
3. 导出到 `SourceArt/Exports`，保留骨骼、UV、法线、材质槽和碰撞命名。
4. 通过 Blender MCP 或 Unreal 导入流程进入 `Content`。
5. 在 Unreal 检查缩放、朝向、LOD、Socket、物理资产和材质。
6. 运行资产验证并记录导入报告。

## Studio 资产工作区

- 资产页扫描 `Content` 中的 `.uasset` 和 `.umap`，可按类型、设计状态和关键词筛选。
- 可多选资产，统一设置所属系统、用途、状态、标签和说明。
- 新源文件先上传到 `SourceArt/Imports/Incoming`，确认目标目录后再导入。
- 导入通过 UE5.8 `PythonScript` commandlet 执行 `AssetImportTask`。
- 导入脚本、日志和结果保存在 `Saved/SoulForgeStudio/asset-imports`。
- 导入失败时保留源文件，并可在 Studio 中查看日志或重试。

## 命名

- 静态网格：`SM_`
- 骨骼网格：`SK_`
- 材质：`M_`
- 材质实例：`MI_`
- 纹理：`T_`
- 动画：`A_` 或 `MM_` / `MF_`
- Blueprint：`BP_`
- 控件：`WBP_`

## Git LFS

以下文件必须由 LFS 管理：

- `.uasset`、`.umap` 和 UE 包文件
- `.blend`、`.fbx`、`.obj`、`.glb`、`.usd`
- 常见纹理、音频和视频格式
- `.zip` 交付包

提交前运行：

```powershell
git lfs status
git lfs ls-files
```

## 验收

- 不带退化的缩放和负缩放。
- UV、法线、切线和材质槽完整。
- 骨骼层级、命名和根骨骼方向一致。
- 碰撞、Socket、LOD 和物理资产经过编辑器验证。
- 源文件、导出文件和 Unreal 资产都能追溯。
