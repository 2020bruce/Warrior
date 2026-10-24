---
name: soulslike-verification
description: 用于在报告完成前验证 UE5.8 魂类项目的编译、Blueprint 与 StateTree、PIE 行为、日志、资产、Git LFS 和性能。
metadata:
  short-description: 魂类项目证据与验证
---

# 魂类项目验证

必须使用最新证据。只检查源码不能作为完成证据。

## 基础检查

- 优先运行 `soulforge doctor`。
- 验证 `.uproject`、`.uplugin`、TOML 和 JSON 语法。
- 添加二进制资产前运行 `git lfs status`。
- 测试依赖编辑器状态时，确认 UE MCP 已连接。
- 测试涉及 Blender 集成时，确认 Blender MCP 的 `9876` 端口正在监听。

## 编译

优先使用项目 CLI：

```powershell
soulforge build
soulforge build --json
```

需要直接调用底层工具时使用：

```powershell
& "<EngineRoot>\Engine\Build\BatchFiles\Build.bat" <ProjectName>Editor Win64 Development -Project="<ProjectRoot>\<ProjectName>.uproject" -WaitMutex
```

如果正在运行的编辑器锁定了目标二进制，应报告该限制，不能声称编译成功。

## 运行时

- 优先使用 `soulforge test --suite <名称>` 运行自动化测试。
- 测试结果以 UE 日志中的 `TEST COMPLETE. EXIT CODE` 为准。
- CLI 测试日志写入 `Saved/Logs/soulforge-cli/test-*.log`。
- 使用命名明确的 PIE 场景、Functional Test 或 AutomationSpec。
- 检查 `Saved/Logs/<ProjectName>.log`，并使用 UE MCP 的 `LogsToolset`。
- 检查新增的 Ensure、Error、资产加载、导航和 Blueprint 编译错误。
- 修改镜头或 UI 时，检查多种视口尺寸。

## Blender

- 运行 `blender.exe --version`。
- 验证插件已启用。
- 确认 Blender MCP 只在 UI 模式启动。
- 将资产重新导入 Unreal，并验证缩放、朝向、枢轴、材质、碰撞和动画。

## 报告

返回 PASS、CONCERNS 或 FAIL，并附上：

- 命令或测试路径；
- 时间；
- 相关输出或日志；
- 生成的产物路径；
- 无法执行的检查。

需要保留项目上下文时运行：

```powershell
soulforge context
```

