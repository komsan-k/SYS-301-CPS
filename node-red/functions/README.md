# Node-RED Lab Series: Writing Functions (JavaScript in the Function node)

A hands-on lab sequence to master **Node-RED Function nodes** and the **`msg` object**—from basic message edits to multi-output routing, async flows, lifecycle hooks, logging, context, and status indicators.

---

## Audience & prerequisites

- **Beginner → intermediate** Node-RED users  
- You should already know how to:
  - **Deploy** flows
  - Use **Inject** and **Debug** nodes
- Tools:
  - **Node-RED**
  - **Function** node (built-in)
  - **Debug** node (built-in)

---

## What you will learn

By completing Labs 0–10, you will be able to:

- Safely read/modify `msg` (especially `msg.payload`)
- Control flow using `return msg`, `return null`, and return arrays
- Route messages using **multiple outputs** and `node.outputCount`
- Emit **multiple messages** per single input
- Implement **asynchronous** logic using `node.send()` and finalize with `node.done()`
- Use Function node **On Start / On Stop** lifecycle hooks
- Debug effectively with `node.log`, `node.warn`, and `node.error` + **Catch** nodes
- Persist state using **context / flow / global** storage
- Provide runtime feedback using `node.status()`

---

## Grading / deliverables (recommended)

For each lab, submit:

1. **Flow export (JSON)**  
2. **Screenshots** of Debug output (or brief notes)  
3. Short reflection: **“What changed in `msg`?”**

> Tip: Put reflections in a single file (e.g., `reflection.md`) with one section per lab.

---

## How to run the labs

1. Create a new flow tab per lab (recommended), or keep them all in one workspace.
2. Build the flow as listed (Inject → Function → Debug, plus any extras).
3. Click **Inject**, watch output in the **Debug** sidebar.
4. Iterate until your output matches the expected behavior.
5. Export the flow: **Menu → Export → Current flow / Selected nodes**.

---

## Lab 0 — “Hello msg” and Function rules

### Goal
Understand `msg`, `msg.payload`, and the rule **“must return a msg object”**.

### Build
**Inject → Function → Debug**

### Tasks
- Pass through: `return msg;`
- Modify payload (string → string)
- Try returning `null` and observe the flow stops

### Deliverable
Explain what happens when returning `null` vs returning `msg`.

---

## Lab 1 — Editing msg safely (don’t break important properties)

### Goal
Learn why constructing a brand-new message can break some flows (e.g., HTTP request/response patterns).

### Build
**Inject → Function → Debug**

### Tasks
- Create a new object:
  ```js
  var newMsg = { payload: msg.payload.length };
  return newMsg;
  ```
- Then do the “safe way”: preserve original `msg`, add new field:
  ```js
  msg.len = msg.payload.length;
  return msg;
  ```

### Deliverable
Compare Debug output and explain the difference.

---

## Lab 2 — Branching logic with multiple outputs

### Goal
Route messages to different outputs using **return arrays**.

### Build
- Inject (topic=`banana`) and Inject (topic=`apple`)
- Function (**2 outputs**) → Debug1 + Debug2

### Task
Implement topic routing:
- `banana` → output 2  
- otherwise → output 1

### Deliverable
Test both topics and show the two Debug panes.

---

## Lab 3 — Generic routing using `node.outputCount`

### Goal
Write a function that adapts when the number of outputs changes.

### Build
**Inject → Function (N outputs; try 3 then 5) → Debug nodes**

### Task
Randomly send a message to one output using `node.outputCount`.

### Deliverable
Change output count without changing code; show it still works.

---

## Lab 4 — Generating multiple messages

### Goal
Emit **multiple messages per single input** (batch expand).

### Build
- Inject payload: `"node red function labs are fun"`
- Function (1 output) → Debug

### Task
Split payload into words and output one message per word.

### Deliverable
Debug shows multiple messages arriving one by one.

---

## Lab 5 — Asynchronous messaging with `node.send()`

### Goal
Learn async pattern: **don’t `return msg`** at the end. Use `node.send()`.

### Build
**Inject → Function → Debug**

### Task
Simulate async work with `setTimeout`:
- On timer, modify `msg.payload` and `node.send(msg);`
- Use `return;` at the end of the main function (no synchronous return)

### Deliverable
Explain why `return msg` is not correct for async.

---

## Lab 6 — Finish tracking with `node.done()`

### Goal
Track completion correctly for async functions.

### Build
Continue from Lab 5.

### Task
Add `node.done()` after `node.send(msg)` in the async callback.

### Deliverable
Short note: what `node.done()` is for.

---

## Lab 7 — On Start / On Stop: initialize and tidy up

### Goal
Use the Function node’s lifecycle hooks.

### Build
**Inject → Function → Debug**

### Tasks
- In **On Start**, initialize a counter in context if missing
- In main function, increment and output count
- In **On Stop**, log a message (or clear a timer if you created one)

### Deliverable
Show the counter survives multiple Inject clicks; resets only when you redeploy/restart (depending on context store).

---

## Lab 8 — Logging and debugging like a pro

### Goal
Use `node.log`, `node.warn`, `node.error` appropriately.

### Build
**Inject → Function → Debug**  
Add a **Catch** node for error testing.

### Tasks
- Use `node.warn("...")` for helpful debugging
- Trigger an error with `node.error("hit an error", msg)` so a Catch node can detect it
- Add a Catch node to confirm behavior

### Deliverable
Screenshot of Catch receiving the error.

---

## Lab 9 — Context storage: node / flow / global

### Goal
Store state and retrieve it safely (sync patterns).

### Build
**Inject → Function → Debug**

### Tasks
- Count how many times the Function ran using `context.get/set`
- Try multi-get/multi-set style patterns such as:
  - `flow.get(["count","colour"])`
  - `flow.set(["count","colour"], [c, col])`

### Deliverable
Debug shows `msg.count` increasing each click.

---

## Lab 10 — Status lights + mini project

### Goal
Make your Function node “self-reporting” using `node.status()` and build a small real flow.

### Mini project: Smart Threshold Router

### Build
**Inject (number) → Function (3 outputs) → Debugs**

### Behavior
- payload < low → output 1 (status: **LOW**)
- low ≤ payload < high → output 2 (status: **MID**)
- payload ≥ high → output 3 (status: **HIGH**)

### Add-ons (optional)
- Store thresholds in **flow** context
- Add error if payload is not numeric (`node.error(..., msg)` + Catch)

### Deliverable
Flow export + short demo notes.

---


---

## Lab 11 — Using external modules with `functionGlobalContext`

### Goal
Learn how to access **external Node.js modules** inside a Function node via `functionGlobalContext`.

### Background
By default, Function nodes run in a sandbox. To use external libraries (e.g., `moment`, `lodash`, custom utilities), they must be exposed in `settings.js`.

### Build
**Inject → Function → Debug**

### Tasks
1. Edit `settings.js` and add:
   ```js
   functionGlobalContext: {
       moment: require("moment")
   }
   ```
2. Restart Node-RED.
3. In the Function node, use:
   ```js
   const moment = global.get("moment");
   msg.payload = moment().format("YYYY-MM-DD HH:mm:ss");
   return msg;
   ```

### Deliverable
- Screenshot of Debug showing formatted timestamp
- Short explanation of why `functionGlobalContext` is required

### Safety note
Explain why unrestricted `require()` is disabled by default (security and stability).

---

## Lab 12 — Preserving `msg.req` / `msg.res` in HTTP flows

### Goal
Understand why **preserving the original `msg` object** is critical in HTTP-based flows.

### Build
**HTTP In → Function → HTTP Response**

(Optional: add Debug after Function)

### Tasks
1. Create an HTTP endpoint:
   - Method: GET
   - URL: `/lab12/test`
2. In the Function node, try this (incorrect):
   ```js
   return { payload: "Hello HTTP" };
   ```
3. Observe the error or hanging request.
4. Fix it by preserving the original message:
   ```js
   msg.payload = "Hello HTTP";
   return msg;
   ```

### Deliverable
- Explanation of what happens when `msg.req` / `msg.res` are lost
- Screenshot of successful browser response after fixing the code

### Key insight
Some Node-RED nodes (HTTP In/Response, WebSocket, TCP) rely on **hidden message properties**. Replacing `msg` breaks the protocol chain.

---

## Extension ideas (optional)

- Modify Lab 12 to return JSON and HTTP status codes
- Combine Lab 11 + Lab 12 to format HTTP responses with an external library
- Discuss best practices for secure Function node design in production flows

## References

1. Node-RED User Guide — **Writing Functions** (Function node, `msg`, multiple outputs, async `node.send()`/`node.done()`, context, logging, status):  
   https://nodered.org/docs/user-guide/writing-functions

2. Node-RED Documentation — User Guide (general guides and concepts):  
   https://nodered.org/docs/user-guide/


