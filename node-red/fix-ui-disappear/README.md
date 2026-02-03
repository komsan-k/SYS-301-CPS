# 🛠 Fixing Node-RED Dashboard Disappearance (Windows)

This guide explains how to completely remove and reinstall Node-RED on Windows to fix the issue where the Node-RED Dashboard (`/ui`) does not appear or stays on the welcome page.

---

## ❗ Problem Summary

**Symptoms**
- `http://127.0.0.1:1880/ui` shows nothing or only a welcome page
- Node-RED editor works, but Dashboard widgets do not render

**Root Cause**
- Using an unsupported Node.js version (e.g., Node.js v23)

---

## ✅ Correct Fix Strategy (IMPORTANT)

Follow this order strictly:

1. Uninstall Node-RED completely  
2. Fix Node.js version (use Node.js 18 LTS)  
3. Reinstall Node-RED  
4. Reinstall Node-RED Dashboard  

---

## 🧹 Step 1: Stop Node-RED (If Running)

If Node-RED is running in a terminal:

```
Ctrl + C
```

---

## 🗑 Step 2: Uninstall Node-RED (Global)

Open **Command Prompt (Run as Administrator)**:

```bat
npm uninstall -g node-red
```

Verify removal:

```bat
node-red --version
```

If you see *command not found*, Node-RED is removed.

---

## 🧨 Step 3: Remove Node-RED User Data (VERY IMPORTANT)

Delete the Node-RED user directory:

```
C:\Users\june\.node-red
```

Or via command line:

```bat
rmdir /s /q %USERPROFILE%\.node-red
```

---

## 🧽 Step 4: Clean npm Cache (Recommended)

```bat
npm cache clean --force
```

---

## 🔍 Step 5: Confirm Node-RED Is Fully Removed

```bat
where node-red
```

If nothing is returned, Node-RED is fully uninstalled.

---

## 🚨 Critical Warning

Node.js v23 is **NOT supported** by Node-RED Dashboard.

Do **NOT** reinstall Node-RED until Node.js is fixed.

---

## ✅ Step 6: Install Node.js 18 LTS

Recommended: use **nvm-windows**.

Verify:

```bat
node -v
```

Expected:

```
v18.x.x
```

---

## 🔁 Step 7: Reinstall Node-RED

```bat
npm install -g --unsafe-perm node-red
```

Start Node-RED:

```bat
node-red
```

---

## 📊 Step 8: Install Node-RED Dashboard

```bat
cd %USERPROFILE%\.node-red
npm install node-red-dashboard
```

Restart Node-RED.

---

## 🌐 Step 9: Test Dashboard

1. Open editor: `http://127.0.0.1:1880`
2. Add `ui_button` or `ui_text`
3. Assign **Tab** and **Group**
4. Deploy
5. Open dashboard: `http://127.0.0.1:1880/ui`

---

## 🎯 Final Check

```bat
node -v
where node
```

---

## ✅ Recommended Stable Setup

| Component | Version |
|---------|--------|
| Node.js | 18 LTS |
| Node-RED | 3.x / 4.x |
| Dashboard | node-red-dashboard |

---

**This procedure resolves dashboard disappearance in almost all Windows systems.**
