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

- 资产页扫描 `Content` 中的 `.uasset` 和 `.umap`，可按类型和关键词筛选。
- 资产详情可直接在 Unreal 中打开，并在有 Blender 源文件时切换到 Blender 查看。
- 新源文件先上传到 `SourceArt/Imports/Incoming`，确认目标目录后再导入。
- 导入通过 UE5.8 `PythonScript` commandlet 执行 `AssetImportTask`。
- 导入脚本、日志和结果保存在 `Saved/SoulForgeStudio/asset-imports`。
- 导入失败时保留源文件，并可在 Studio 中查看日志或重试。
- “Blender 制作”页签会启动 `@asset-production-engineer`，使用 Blender 4.5.15 生成源资产。
- Agent 结果清单保存在 `Saved/SoulForgeStudio/asset-productions`。
- FBX 会自动登记到导入队列；开启自动导入后由 UE commandlet 写入 `Content`。
- 自动导入成功的静态网格会写入 `.soulforge/assets.json`，默认标记为 Blender 生成。

## 角色资产示例

`SM_Doraemon_Base` 是当前的可绑定分件角色基础模型：

- 31 个独立网格部件、7 个共享材质，全部保留 UV 和材质槽。
- 总高度 1.29m，脚底统一位于世界原点，旋转和缩放已应用。
- 非流形边、零面积面、松散顶点和负体积对象均为 0。
- FBX 往返验证保持对象数量、尺寸、UV 和材质槽一致。
- 当前尚未创建 Armature、骨骼、权重和动画，进入动画生产前需要完成绑定。
- 源文件位于 `SourceArt/Blender/Characters/SM_Doraemon_Base.blend`，UE 资产位于
  `Content/Assets/Blender/characters`。

`SM_Doraemon_Showcase` 是当前游戏主角使用的展示网格：

- 单个合并网格，8 个材质槽，尺寸约 `1.108 × 0.806 × 1.290m`。
- 脸部、肚皮和口袋使用贴合头部与身体曲面的面板，眼睛、瞳孔和高光分层。
- 非流形边、零面积面和游离点均为 0，保留 UV 和完整材质分配。
- 源文件位于 `SourceArt/Blender/Characters/SM_Doraemon_Showcase.blend`。
- 当前由 `AWarriorHeroCharacter` 的 `DoraemonMeshComponent` 显示。
- 这是绑定前静态网格；移动和碰撞沿用 ACharacter，但尚未接入骨骼动画。

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
