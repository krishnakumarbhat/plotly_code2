let sessionId = window.localStorage.getItem("rag_session_id") || "";

const chatWindow = document.getElementById("chatWindow");
const chatForm = document.getElementById("chatForm");
const questionInput = document.getElementById("questionInput");
const sendBtn = document.getElementById("sendBtn");
const newSessionBtn = document.getElementById("newSessionBtn");

function autoResizeInput() {
  questionInput.style.height = "auto";
  questionInput.style.height = `${Math.min(questionInput.scrollHeight, 180)}px`;
}

function addMessage(role, content) {
  const el = document.createElement("div");
  el.className = `message ${role}`;
  el.textContent = content;
  chatWindow.appendChild(el);
  chatWindow.scrollTop = chatWindow.scrollHeight;
}

async function askQuestion(question) {
  const payload = { question };
  if (sessionId) payload.session_id = sessionId;

  const response = await fetch("/ask", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(payload),
  });

  if (!response.ok) {
    throw new Error(`Request failed: ${response.status}`);
  }

  return response.json();
}

chatForm.addEventListener("submit", async (event) => {
  event.preventDefault();
  const question = questionInput.value.trim();
  if (!question) return;

  addMessage("user", question);
  questionInput.value = "";
  autoResizeInput();
  sendBtn.disabled = true;

  try {
    const data = await askQuestion(question);
    if (data.session_id) {
      sessionId = data.session_id;
      window.localStorage.setItem("rag_session_id", sessionId);
    }
    addMessage("assistant", data.answer || "No answer generated.");
  } catch (error) {
    addMessage("assistant", `Error: ${error.message}`);
  } finally {
    sendBtn.disabled = false;
    questionInput.focus();
  }
});

questionInput.addEventListener("input", autoResizeInput);

questionInput.addEventListener("keydown", (event) => {
  if (event.key === "Enter" && !event.shiftKey) {
    event.preventDefault();
    chatForm.requestSubmit();
  }
});

newSessionBtn.addEventListener("click", () => {
  sessionId = "";
  window.localStorage.removeItem("rag_session_id");
  chatWindow.innerHTML = "";
  addMessage("assistant", "Started a new session. Ask your next question.");
});

addMessage(
  "assistant",
  "Welcome. I keep chat context per session. Ask about your ingested documents."
);

autoResizeInput();
