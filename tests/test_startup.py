"""Check startup and request failures against an actual compiled executable."""

import os
from pathlib import Path
import subprocess
import sys
import unittest


BINARY = Path(sys.argv.pop(1)).resolve()


class StartupTests(unittest.TestCase):
    def run_program(self, *args, env=None):
        return subprocess.run(
            [str(BINARY), *args], capture_output=True, text=True, timeout=10, env=env
        )

    def test_missing_username(self):
        result = self.run_program()
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("Usage:", result.stdout + result.stderr)

    def test_empty_username(self):
        result = self.run_program("")
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("Usage:", result.stdout + result.stderr)

    def test_request_failure(self):
        env = os.environ.copy()
        # Unusable local proxy: deterministic failure without contacting GitHub.
        for name in ("HTTP_PROXY", "HTTPS_PROXY", "ALL_PROXY"):
            env[name] = env[name.lower()] = "http://127.0.0.1:0"
        env["NO_PROXY"] = env["no_proxy"] = ""
        result = self.run_program("octocat", env=env)
        self.assertEqual(result.returncode, 1, result.stderr)
        self.assertIn("Error:", result.stderr)
        self.assertNotIn("terminating due to uncaught exception", result.stderr)


if __name__ == "__main__":
    unittest.main()
