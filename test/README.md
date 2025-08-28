# Blog Application Test Suite

This test suite provides comprehensive testing for the Simple Blog Application, including both API endpoints and web pages.

## Features

### API Testing

- **CRUD Operations**: Create, Read, Update, Delete blog posts
- **Authentication**: Tests admin authentication for protected endpoints
- **Performance**: Concurrent request testing with configurable parameters
- **Security**: Various security tests including SQL injection, XSS, and unauthorized access

### Web Page Testing

- **Static Pages**: Tests HTML endpoints (home, admin login)
- **Dynamic Pages**: Tests individual blog post pages
- **Admin Authentication**: Tests login functionality

## Prerequisites

- Node.js (v14 or higher)
- npm or yarn
- Running blog application server on http://localhost:8080

## Installation

```bash
cd test
npm install
```

## Configuration

Edit the `CONFIG` object in `test_api.js` to customize test parameters:

```javascript
const CONFIG = {
  baseUrl: "http://localhost:8080", // Server URL
  numBlogs: 50, // Number of blogs to create for testing
  concurrentRequests: 20, // Concurrent requests per test
  iterations: 5, // Number of iterations for each test
  delayBetweenTests: 500, // Delay between tests in ms
  timeout: 10000, // Request timeout in ms
  enableSecurityTests: true, // Whether to include security tests
  adminToken: "Bearer admin-token-123", // Admin auth token for API tests
};
```

## Running Tests

### Run All Tests

```bash
npm test
```

### Run Individual Test

```bash
node test_api.js
```

## Test Categories

### 1. Web Pages Test

- Tests HTML endpoints
- Verifies page accessibility
- Checks response times

### 2. API Performance Tests

- **Create Blogs**: Tests blog creation via API
- **Read Single Blog**: Tests individual blog retrieval
- **Read All Blogs**: Tests bulk blog retrieval
- **Update Blog**: Tests blog modification
- **Delete Blog**: Tests blog deletion

### 3. Authentication Tests

- Tests admin login functionality
- Verifies protected endpoints require authentication
- Tests both cookie and token-based authentication

### 4. Security Tests

- SQL Injection attempts
- XSS payload testing
- Invalid JSON handling
- Oversized payload testing
- Malformed ID parameters
- Method not allowed testing
- Invalid content type testing
- Non-existent endpoint testing
- Unauthorized access attempts

## Test Results

The test suite provides detailed statistics including:

- Success/failure rates
- Response times (min, max, average)
- Performance metrics per endpoint
- Overall summary

Example output:

```
API CREATE OPERATIONS:
Total Requests: 50
Success: 50 (100.00%)
Failed: 0
Average Response Time: 45.23 ms
Min Response Time: 23 ms
Max Response Time: 127 ms
```

## Understanding Security Tests

For security tests, a "success" means the server **correctly rejected** a malicious request:

- ✅ Success: Server returns 4xx/5xx status for malicious input
- ❌ Failure: Server accepts malicious input (security vulnerability)

## Troubleshooting

### Server Not Running

If you get connection errors, ensure the blog server is running:

```bash
cd ..
./build.sh
./build/simple_blog
```

### Authentication Failures

If API tests fail with 401 errors, verify:

1. Admin authentication is working
2. The auth token in CONFIG matches the server's expected token
3. Cookie-based auth is properly implemented

### High Failure Rates

If tests show high failure rates:

1. Check server logs for errors
2. Verify database connectivity
3. Ensure sufficient server resources
4. Check network connectivity

## Extending Tests

To add new test types:

1. Add new stat category to `stats` object
2. Create test function following the pattern
3. Add to main test execution flow
4. Update results printing function

Example:

```javascript
async function myNewTest(blogs) {
  return await timedRequest(
    `${CONFIG.baseUrl}/my/endpoint`,
    { method: "GET" },
    "my_test_category"
  );
}
```
