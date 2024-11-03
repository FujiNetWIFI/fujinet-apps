import puppeteer from "puppeteer";
import UserAgent from "user-agents";
import fs from "fs";

async function run() {
    const browser = await puppeteer.launch();
    const page = await browser.newPage();
    const userAgent = new UserAgent().random().toString();
  
    await page.setUserAgent(userAgent);
    await page.goto("https://www.bloomberg.com/graphics/2024-us-election-results", { waitUntil: 'networkidle2' });
    const pageContent = await page.content();
    fs.writeFileSync("/var/www/html/2024pres/live.html", pageContent, "utf-8");
    console.log("Page content saved as live.html");
    await browser.close();
}

run();