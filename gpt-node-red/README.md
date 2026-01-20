
# node-red-contrib-custom-chatgpt
### Using ChatGPT inside Node-RED Flows (Text & Image Generation)

`node-red-contrib-custom-chatgpt` is a **Node-RED custom node** that enables integration with **OpenAI models** (e.g., GPT‑4o) for **text generation (chat/completions)** and **image generation (DALL·E)** directly inside Node-RED flows.

The node allows developers and students to build **AI-augmented workflows**, dashboards, and CPS/IoT applications by controlling behavior through standard Node-RED message properties.

---

## 1. Core Concept

The node behavior is primarily controlled using:

- **`msg.topic`** → selects the *mode or model*
- **`msg.payload`** → contains the *prompt* or *image description*

This design follows Node-RED’s message-driven programming paradigm and allows easy integration with sensors, dashboards, and automation logic.

---

## 2. Supported Functions

### 2.1 Text Generation (Chat / Completion)

- Set:
  ```javascript
  msg.topic = "gpt4o";
  msg.payload = "Your prompt here";
  ```
- The node sends the prompt to an OpenAI chat model and returns the generated text in `msg.payload`.

Typical use cases:
- CPS / IoT status summarization  
- Natural language explanations for dashboards  
- Chatbots and teaching assistants  

---

### 2.2 Image Generation (DALL·E)

- Set:
  ```javascript
  msg.topic = "image";
  msg.payload = "A diagram of a cyber-physical system";
  ```

Optional properties:
- `msg.size` → image resolution (e.g., `"1024x1024"`)
- `msg.format` → image format (e.g., `"png"`)

The generated image is returned as a Base64-encoded payload or URL (depending on node configuration).

---

### 2.3 Optional Parameters

To fine-tune generation behavior, the following optional properties can be added:

- `msg.max_tokens` – limits response length  
- `msg.temperature` – controls creativity (0.0–1.0)  
- `msg.top_p` – nucleus sampling parameter  
- `msg.history` – conversation context (for chatbots)

Example:
```javascript
msg.temperature = 0.3;
msg.max_tokens = 120;
```

---

## 3. Installation

### Option A: Node-RED Palette Manager
1. Open Node-RED
2. Menu → **Manage palette** → **Install**
3. Search for:
   ```
   node-red-contrib-custom-chatgpt
   ```

### Option B: Command Line
```bash
npm install node-red-contrib-custom-chatgpt
```

---

## 4. OpenAI API Key

An **OpenAI API key** is required.

Recommended method:
- Configure the API key directly in the **ChatGPT (custom)** node settings

Alternative method:
- Pass dynamically using:
  ```javascript
  msg.API_KEY = "your_api_key_here";
  ```

---

## 5. Simple Working Example (Text Q&A)

### Objective
Send a question to GPT‑4o and display the response in the Debug sidebar.

### Flow Logic
1. **Inject** node sends a question string  
2. **Function** node sets `msg.topic` and `msg.payload`  
3. **ChatGPT** node calls OpenAI  
4. **Debug** node displays the response  

### Importable Flow (JSON)

```json
[
  {
    "id": "a1_inject",
    "type": "inject",
    "name": "Ask",
    "payload": "Explain CPS in 3 bullet points and give 1 example.",
    "payloadType": "str",
    "wires": [["a2_settopic"]]
  },
  {
    "id": "a2_settopic",
    "type": "function",
    "name": "Set msg.topic = gpt4o",
    "func": "msg.topic = "gpt4o"; return msg;",
    "wires": [["a3_chatgpt"]]
  },
  {
    "id": "a3_chatgpt",
    "type": "chatgpt",
    "name": "ChatGPT (custom)",
    "wires": [["a4_debug"]]
  },
  {
    "id": "a4_debug",
    "type": "debug",
    "name": "Answer",
    "complete": "payload"
  }
]
```

Deploy the flow and click **Ask** to see the generated response.

---

## 6. Best Practices

- Use **specific prompts** (format, length, tone)
- Keep **control logic deterministic** (AI for explanation, not safety-critical control)
- Use `msg.history` for conversational continuity
- Limit token usage for dashboards and real-time CPS systems

---

## 7. References

1. Node-RED Flow Library – *node-red-contrib-custom-chatgpt*  
   https://flows.nodered.org/node/node-red-contrib-custom-chatgpt

2. Node-RED Documentation  
   https://nodered.org/docs/

3. OpenAI API Documentation  
   https://platform.openai.com/docs

4. O. Givehchi et al., “Cyber-Physical Systems: Architecture, Design, and Applications,” *IEEE Industrial Electronics Magazine*, 2019.

5. K. Kanjanasit et al., “Cyber-Physical Systems and AI-Assisted Monitoring Using Node-RED,” educational material, Prince of Songkla University.

---

## 8. Typical Applications

- ESP32 / IoT sensor summarization  
- CPS dashboards with AI explanations  
- Educational chatbots for engineering courses  
- AI-assisted monitoring and reporting systems  


