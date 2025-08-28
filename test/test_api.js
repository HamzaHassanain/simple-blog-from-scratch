/**
 * Comprehensive Test Suite for Simple Blog Application
 *
 * This script tests both the API endpoints and web pages of the blog application
 * by sending HTTP requests and measuring response times and success rates.
 * It includes tests for CRUD operations on blogs, authentication, and security testing.
 *
 * Requirements:
 * - Node.js
 * - npm packages: node-fetch, cli-progress, colors
 *
 * Install dependencies:
 * npm install node-fetch@2 cli-progress colors
 *
 * Run the test:
 * node test_api.js
 */

const fetch = require("node-fetch");
const cliProgress = require("cli-progress");
const colors = require("colors");

// Configuration
const CONFIG = {
  baseUrl: "http://localhost:8080",
  numBlogs: 50, // Number of blogs to create for testing
  concurrentRequests: 20, // Number of concurrent requests per test
  iterations: 5, // Number of iterations for each test
  delayBetweenTests: 500, // Delay between tests in ms
  timeout: 1000, // Request timeout in ms
  enableSecurityTests: true, // Whether to include security tests
  adminToken: "Bearer admin-token-123", // Admin auth token for API tests
};

// Test statistics
const stats = {
  api_create: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  api_read: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  api_readAll: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  api_update: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  api_delete: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  web_pages: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  admin_auth: {
    success: 0,
    fail: 0,
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
  security: {
    success: 0, // For security tests, success means the server properly rejected the request
    fail: 0, // For security tests, fail means the server improperly accepted the request
    totalTime: 0,
    maxTime: 0,
    minTime: Number.MAX_SAFE_INTEGER,
  },
};

// Generate a random blog post
function generateBlog(index) {
  const titles = [
    "Getting Started with Web Development",
    "Understanding Modern JavaScript",
    "The Future of Technology",
    "Building Scalable Applications",
    "Introduction to Database Design",
    "Mastering CSS Grid",
    "API Design Best Practices",
    "Security in Web Applications",
    "Performance Optimization Tips",
    "Mobile-First Development",
  ];

  const contentTemplates = [
    "This is a comprehensive guide about {topic}. We'll explore various aspects and provide practical examples. The content covers fundamental concepts, advanced techniques, and real-world applications. Whether you're a beginner or an experienced developer, this post will provide valuable insights.",
    "In this detailed article, we dive deep into {topic}. You'll learn about the latest trends, best practices, and common pitfalls to avoid. We'll also discuss implementation strategies and provide code examples to help you understand the concepts better.",
    "Welcome to our exploration of {topic}. This post aims to provide a thorough understanding of the subject matter through practical examples and expert insights. We'll cover everything from basic principles to advanced implementation techniques.",
  ];

  const title = titles[Math.floor(Math.random() * titles.length)] + ` ${index}`;
  const template =
    contentTemplates[Math.floor(Math.random() * contentTemplates.length)];
  const content =
    template.replace("{topic}", title.toLowerCase()) +
    ` Additional content for uniqueness: ${Math.random()
      .toString(36)
      .substring(2, 15)}`;

  return {
    title: title,
    content: content,
  };
}

// Perform a single request with timing
async function timedRequest(url, options, testType) {
  const startTime = Date.now();

  try {
    const response = await Promise.race([
      fetch(url, options),
      new Promise((_, reject) =>
        setTimeout(() => reject(new Error("Request timeout")), CONFIG.timeout)
      ),
    ]);

    const endTime = Date.now();
    const elapsed = endTime - startTime;

    // For security tests, a failure (4xx/5xx) is actually a success
    if (testType === "security") {
      if (!response.ok) {
        // The server correctly rejected the malicious request
        stats[testType].success++;
        stats[testType].totalTime += elapsed;
        stats[testType].maxTime = Math.max(stats[testType].maxTime, elapsed);
        stats[testType].minTime = Math.min(stats[testType].minTime, elapsed);
        return { securityTestSuccess: true, status: response.status };
      } else {
        // The server incorrectly accepted the malicious request

        stats[testType].fail++;
        console.error(
          "\n",
          colors.red(
            `Security test failed! Server accepted malicious request to ${url}\n`
          )
        );
        return { securityTestFailed: true };
      }
    }

    // Normal request handling (non-security tests)
    if (!response.ok && response.status !== 404) {
      throw new Error(`HTTP error! Status: ${response.status}`);
    }

    // Update statistics
    stats[testType].success++;
    stats[testType].totalTime += elapsed;
    stats[testType].maxTime = Math.max(stats[testType].maxTime, elapsed);
    stats[testType].minTime = Math.min(stats[testType].minTime, elapsed);

    // Special handling for 204 No Content responses - don't try to parse JSON
    if (response.status === 204) {
      return { success: true };
    }

    return await response.json();
  } catch (error) {
    const endTime = Date.now();
    const elapsed = endTime - startTime;

    stats[testType].fail++;
    // console.error(`Error in ${testType} request: ${error.message} \n`, "\n");
    return null;
  }
}

// Create blogs via API
async function createBlogs(count) {
  console.log(`\n${"=".repeat(50)}`);
  console.log(`Creating ${count} blogs via API...`);

  const progressBar = new cliProgress.SingleBar({
    format:
      "Creating Blogs |" +
      colors.cyan("{bar}") +
      "| {percentage}% || {value}/{total} blogs",
    barCompleteChar: "\u2588",
    barIncompleteChar: "\u2591",
    hideCursor: true,
  });

  progressBar.start(count, 0);

  const createdBlogs = [];

  for (let i = 0; i < count; i++) {
    const blog = generateBlog(i);
    const response = await timedRequest(
      `${CONFIG.baseUrl}/api/blogs`,
      {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
          Authorization: CONFIG.adminToken,
        },
        body: JSON.stringify(blog),
      },
      "api_create"
    );

    if (response && response.blog) {
      createdBlogs.push(response.blog);
    }

    progressBar.update(i + 1);
  }

  progressBar.stop();
  console.log(`Created ${createdBlogs.length} blogs successfully.`);

  return createdBlogs;
}

// Run multiple concurrent requests for a specific test type
async function runConcurrentRequests(testType, blogs, requestFn) {
  console.log(`\n${"=".repeat(50)}`);
  console.log(
    `Running ${testType} test with ${CONFIG.concurrentRequests} concurrent requests x ${CONFIG.iterations} iterations`
  );

  const progressBar = new cliProgress.SingleBar({
    format:
      `${testType} Test |` +
      colors.cyan("{bar}") +
      "| {percentage}% || {value}/{total} requests",
    barCompleteChar: "\u2588",
    barIncompleteChar: "\u2591",
    hideCursor: true,
  });

  const totalRequests = CONFIG.concurrentRequests * CONFIG.iterations;
  progressBar.start(totalRequests, 0);

  let completed = 0;

  for (let i = 0; i < CONFIG.iterations; i++) {
    const requests = Array.from({ length: CONFIG.concurrentRequests }, () => {
      const promise = requestFn(blogs).then(() => {
        progressBar.update(++completed);
      });
      return promise;
    });

    await Promise.all(requests);

    if (i < CONFIG.iterations - 1) {
      await new Promise((resolve) =>
        setTimeout(resolve, CONFIG.delayBetweenTests)
      );
    }
  }

  progressBar.stop();
}

// API Test Functions

// Read single blog test
async function readBlogTest(blogs) {
  const randomIndex = Math.floor(Math.random() * blogs.length);
  const blog = blogs[randomIndex];

  if (!blog) return;

  return await timedRequest(
    `${CONFIG.baseUrl}/api/blogs/${blog.id}`,
    {
      method: "GET",
    },
    "api_read"
  );
}

// Read all blogs test
async function readAllBlogsTest() {
  return await timedRequest(
    `${CONFIG.baseUrl}/api/blogs`,
    {
      method: "GET",
    },
    "api_readAll"
  );
}

// Update blog test
async function updateBlogTest(blogs) {
  const randomIndex = Math.floor(Math.random() * blogs.length);
  const blog = blogs[randomIndex];

  if (!blog) return;

  const updatedBlog = {
    title: `Updated ${blog.title}`,
    content: `Updated content - ${Math.random()
      .toString(36)
      .substring(2, 15)} - ${blog.content}`,
  };

  return await timedRequest(
    `${CONFIG.baseUrl}/api/blogs/${blog.id}`,
    {
      method: "PUT",
      headers: {
        "Content-Type": "application/json",
        Authorization: CONFIG.adminToken,
      },
      body: JSON.stringify(updatedBlog),
    },
    "api_update"
  );
}

// Delete blog test
async function deleteBlogTest(blogs) {
  // Remove a blog from the array and return it
  const randomIndex = Math.floor(Math.random() * blogs.length);
  const blog = blogs.splice(randomIndex, 1)[0];

  if (!blog) return;

  return await timedRequest(
    `${CONFIG.baseUrl}/api/blogs/${blog.id}`,
    {
      method: "DELETE",
      headers: {
        Authorization: CONFIG.adminToken,
      },
    },
    "api_delete"
  );
}

// Web Pages Test Functions

// Test web pages (HTML endpoints)
async function testWebPages() {
  const pages = [
    { url: "/", name: "Home Page" },
    { url: "/admin/login", name: "Admin Login" },
  ];

  for (const page of pages) {
    await timedRequest(
      `${CONFIG.baseUrl}${page.url}`,
      { method: "GET" },
      "web_pages"
    );
  }
}

// Test specific blog pages (requires existing blogs)
async function testBlogPages(blogs) {
  if (blogs.length === 0) return;

  const randomBlog = blogs[Math.floor(Math.random() * blogs.length)];

  return await timedRequest(
    `${CONFIG.baseUrl}/blogs/${randomBlog.id}`,
    { method: "GET" },
    "web_pages"
  );
}

// Test admin authentication
async function testAdminAuth() {
  // Test login page
  await timedRequest(
    `${CONFIG.baseUrl}/admin/login`,
    { method: "GET" },
    "admin_auth"
  );

  // Test login submission (should redirect)
  await timedRequest(
    `${CONFIG.baseUrl}/admin/login`,
    {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body: "username=admin&password=password",
    },
    "admin_auth"
  );
}

// Security Tests
// Generate a random security test for blog endpoints
async function securityTest(blogs) {
  // List of security test types
  const testTypes = [
    "sqlInjection",
    "xss",
    "invalidJson",
    "oversizedPayload",
    "malformedId",
    "methodNotAllowed",
    "invalidContentType",
    "nonExistentEndpoint",
    "unauthorizedAccess",
  ];

  // Randomly select a test type
  const testType = testTypes[Math.floor(Math.random() * testTypes.length)];

  // If blogs array is empty or undefined, use a fallback ID
  const fallbackId = "123456789";
  const randomBlog =
    blogs && blogs.length > 0
      ? blogs[Math.floor(Math.random() * blogs.length)]
      : { id: fallbackId };

  switch (testType) {
    case "sqlInjection":
      // SQL Injection tests
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs/${randomBlog.id}' OR '1'='1`,
        { method: "GET" },
        "security"
      );

    case "xss":
      // XSS tests
      const xssPayload = {
        title: `<script>alert('XSS')</script>`,
        content: `<img src="x" onerror="alert('XSS')"> Blog content with XSS attempt`,
      };
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs`,
        {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
            Authorization: CONFIG.adminToken,
          },
          body: JSON.stringify(xssPayload),
        },
        "security"
      );

    case "invalidJson":
      // Invalid JSON format
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs`,
        {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
            Authorization: CONFIG.adminToken,
          },
          body: `{"title": "Invalid JSON, "content": "Missing quote"}`,
        },
        "security"
      );

    case "oversizedPayload":
      // Oversized payload
      const hugeContent = "x".repeat(1024 * 100); // 100KB of content
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs`,
        {
          method: "POST",
          headers: {
            "Content-Type": "application/json",
            Authorization: CONFIG.adminToken,
          },
          body: JSON.stringify({
            title: "Huge Blog Post",
            content: hugeContent,
          }),
        },
        "security"
      );

    case "malformedId":
      // Malformed ID parameter
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs/null`,
        { method: "GET" },
        "security"
      );

    case "methodNotAllowed":
      // Method not allowed
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs`,
        { method: "PATCH" },
        "security"
      );

    case "invalidContentType":
      // Invalid content type
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs`,
        {
          method: "POST",
          headers: {
            "Content-Type": "text/plain",
            Authorization: CONFIG.adminToken,
          },
          body: "This is not JSON",
        },
        "security"
      );

    case "nonExistentEndpoint":
      // Non-existent endpoint
      return await timedRequest(
        `${CONFIG.baseUrl}/api/nonexistent/${Math.random()
          .toString(36)
          .substring(7)}`,
        { method: "GET" },
        "security"
      );

    case "unauthorizedAccess":
      // Unauthorized access (no auth token)
      return await timedRequest(
        `${CONFIG.baseUrl}/api/blogs`,
        {
          method: "POST",
          headers: { "Content-Type": "application/json" },
          body: JSON.stringify({
            title: "Unauthorized Blog",
            content: "This should be rejected",
          }),
        },
        "security"
      );

    default:
      return null;
  }
}

// Print test results
function printResults() {
  console.log(`\n${"=".repeat(50)}`);
  console.log(colors.bold.underline("BLOG APPLICATION TEST RESULTS"));
  console.log("=".repeat(50));

  const printStats = (type) => {
    const s = stats[type];
    const total = s.success + s.fail;
    const successRate = total > 0 ? ((s.success / total) * 100).toFixed(2) : 0;
    const avgTime = s.success > 0 ? (s.totalTime / s.success).toFixed(2) : 0;

    console.log(
      colors.bold(`\n${type.toUpperCase().replace("_", " ")} OPERATIONS:`)
    );
    console.log(`Total Requests: ${total}`);
    console.log(`Success: ${colors.green(s.success)} (${successRate}%)`);
    console.log(`Failed: ${colors.red(s.fail)}`);
    console.log(`Average Response Time: ${avgTime} ms`);
    if (s.success > 0) {
      console.log(`Min Response Time: ${s.minTime} ms`);
      console.log(`Max Response Time: ${s.maxTime} ms`);
    }
  };

  printStats("api_create");
  printStats("api_read");
  printStats("api_readAll");
  printStats("api_update");
  printStats("api_delete");
  printStats("web_pages");
  printStats("admin_auth");

  if (CONFIG.enableSecurityTests) {
    printStats("security");
  }

  const totalSuccess = Object.values(stats).reduce(
    (sum, s) => sum + s.success,
    0
  );
  const totalFail = Object.values(stats).reduce((sum, s) => sum + s.fail, 0);
  const totalRequests = totalSuccess + totalFail;
  const overallSuccessRate =
    totalRequests > 0 ? ((totalSuccess / totalRequests) * 100).toFixed(2) : 0;

  console.log(`\n${"=".repeat(50)}`);
  console.log(colors.bold.underline("OVERALL SUMMARY:"));
  console.log(`Total Requests: ${totalRequests}`);
  console.log(`Overall Success Rate: ${colors.green(overallSuccessRate)}%`);
  console.log("=".repeat(50));
}

// Main function
async function main() {
  console.log(
    colors.bold.yellow(`
    =========================================
    SIMPLE BLOG APPLICATION - TEST SUITE
    =========================================
    `)
  );

  console.log(
    colors.cyan(`Configuration:
    - Base URL: ${CONFIG.baseUrl}
    - Blogs to create: ${CONFIG.numBlogs}
    - Concurrent requests: ${CONFIG.concurrentRequests}
    - Iterations: ${CONFIG.iterations}
    - Timeout: ${CONFIG.timeout}ms
    - Security tests: ${CONFIG.enableSecurityTests ? "Enabled" : "Disabled"}
    `)
  );

  try {
    // Test web pages first (non-API)
    console.log(colors.bold("\n=== TESTING WEB PAGES ==="));
    await testWebPages();
    await testAdminAuth();

    // Create initial test blogs via API
    console.log(colors.bold("\n=== TESTING API ENDPOINTS ==="));
    const blogs = await createBlogs(CONFIG.numBlogs);

    if (blogs.length === 0) {
      console.error(
        colors.red(
          "Failed to create test blogs. Continuing with other tests..."
        )
      );
    } else {
      // Test blog pages that require existing blogs
      console.log(colors.bold("\n=== TESTING BLOG PAGES ==="));
      for (let i = 0; i < 10; i++) {
        // Test 10 random blog pages
        await testBlogPages(blogs);
      }

      // Make a copy of blogs array that can be modified during the delete test
      let blogsCopy = [...blogs];

      // Run API tests
      console.log(colors.bold("\n=== RUNNING API PERFORMANCE TESTS ==="));

      // Run read single blog test
      await runConcurrentRequests("Read Single Blog", blogs, readBlogTest);

      // Run read all blogs test
      await runConcurrentRequests("Read All Blogs", blogs, readAllBlogsTest);

      // Run update blogs test
      await runConcurrentRequests("Update Blog", blogs, updateBlogTest);

      // Run delete blogs test (using the copy that will be modified)
      await runConcurrentRequests("Delete Blog", blogsCopy, deleteBlogTest);
    }

    // Run security tests if enabled
    if (CONFIG.enableSecurityTests) {
      console.log(colors.bold("\n=== RUNNING SECURITY TESTS ==="));
      await runConcurrentRequests("Security Testing", blogs, securityTest);
    }

    // Print results
    printResults();

    // Clean up any remaining blogs
    console.log("\nCleaning up remaining blogs...");
    for (const blog of blogs) {
      try {
        await fetch(`${CONFIG.baseUrl}/api/blogs/${blog.id}`, {
          method: "DELETE",
          headers: {
            Authorization: CONFIG.adminToken,
          },
        });
      } catch (e) {
        console.error(`Failed to clean up blog ${blog.id}: ${e.message}`);
      }
    }

    console.log(colors.green("\n✅ Test suite completed successfully!"));
  } catch (error) {
    console.error(colors.red(`\nFailed to run test suite: ${error.message}`));
    console.error(error.stack);
  }
}

// Run the test
main().catch((error) => {
  console.error(colors.red(`\nUnhandled error: ${error.message}`));
  console.error(error.stack);
});
