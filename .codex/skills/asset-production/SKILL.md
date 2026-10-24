---
name: asset-production
description: 用于从 Studio 资产需求创建 Blender 模型、保存源文件、导出 FBX/预览并交付 Unreal 导入清单。
metadata:
  short-description: Blender 资产制作与导入交付
---

# 资产制作交付

## 流程

1. 读取 Studio 生成的 prompt 和资产需求。
2. 启动或复用 Blender 4.5.15，优先使用 `blender` MCP。
3. 建立真实几何体、材质、UV 和符合 Unreal 约定的命名。
4. 保存可编辑 `.blend`。
5. 只导出需要导入的对象为 FBX，应用旋转和缩放。
6. 渲染一张能辨认轮廓、材质和比例的预览图。
7. 写入 Studio 指定的 `result.json`。
8. 等待 Studio 执行独立技术验证、多视角渲染和视觉评审。
9. 质量门禁未通过时，按结构化问题在现有资产上做有限轮次优化。
10. 返回生成文件、Blender 检查结果和剩余风险。

## 目录

```text
SourceArt/Blender/<分类>/<资产名>.blend
SourceArt/Exports/<分类>/<资产名>.fbx
SourceArt/Exports/<分类>/<资产名>_preview.png
```

## 禁止

- 不直接创建或修改 `Content` 中的 `.uasset`
- 不使用空物体或未导出的场景冒充资产
- 不覆盖已完成且不在当前需求中的源资产
- 不执行 Git push

## 失败续作

- “继续执行”必须读取上一轮 `result.json`、Agent 日志和质量报告。
- 优先恢复上一轮 Codex 会话；需要新会话时必须在结果中说明原因。
- 优先打开当前 `.blend`，保留用户手工修改和已合格部件，不从零重建。
- Blender MCP 当前场景不是目标资产时，先加载现有 `.blend`；禁止在默认 Cube 场景继续。
- 只修复质量报告和验收标准中的问题，然后重新导出 FBX 和预览。
- “继续验证”不运行 Agent，必须验证磁盘上的最新人工修复文件。
- 续作必须创建新尝试记录，不覆盖原失败日志。

## 验证

- `.blend`、FBX 和预览图均存在且大小非零
- 几何体面数大于零
- 单位、缩放、旋转和原点合理
- 法线朝外，UV 和材质槽无意外丢失
- FBX 导入目标路径和资产名已写入结果清单
- Studio 质量报告通过前，资产不得暂存或导入 Unreal
