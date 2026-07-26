export const DynamicContextPrune = async () => {
  return {
    "experimental.session.compacting": async (input, output) => {
      output.prompt = `You are generating a compressed continuation prompt. Rules:
1. Keep only: current task, modified files, blockers, next steps.
2. Drop: full conversation history, resolved issues, verbose explanations.
3. Be brief — use bullet points. Max 500 tokens.`
    },
  }
}
