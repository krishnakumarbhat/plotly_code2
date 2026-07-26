from app.main import Application
app = Application()
count = app.rag_engine.add_text('session memory verification text', 'smoke', 1)
print('chunks', count)
history = [
    {'role': 'user', 'content': 'My topic is KPI telemetry.'},
    {'role': 'assistant', 'content': 'Your topic is KPI telemetry.'},
]
answer = app.rag_engine.answer('What is my topic?', history=history)
print('answer_ok', len(answer) > 0)
print(answer[:180])
