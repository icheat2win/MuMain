#!/bin/bash
# Run test briefly and capture output

./test_main_simple > test_output.txt 2>&1 &
PID=$!

# Wait 2 seconds
sleep 2

# Kill it
kill $PID 2>/dev/null || true
wait $PID 2>/dev/null || true

# Show output
cat test_output.txt
echo ""
echo "Test was running (PID: $PID)"
