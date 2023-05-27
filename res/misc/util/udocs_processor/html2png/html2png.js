const puppeteer = require('puppeteer');
const argv = require('minimist')(process.argv.slice(2));
const path = require("path");

// C:\Users\cobal\Kommpute\udocs-processor> node html2png/html2png.js
// --input=cmake-build-debug/dist/output/images/
// --output=png --ids="95, 102, 112,"

// args
const InputDir = path.resolve(argv.input) + "/" || ".";
const OutputDir = path.resolve(argv.output) + "/" || "png/";
const Ids = argv.ids.split(/\s*,\s*/) || [];

const ResultExtension = ".png";
const Protocol = "file://";
console.log("Starting converting HTML -> PNG: ", Ids);

(async () => {
    const browser = await puppeteer.launch();
    const page = await browser.newPage();

    for (let Index in Ids) {
        let Id = Ids[Index];
        if (Id !== "") {
            let Url = Protocol + InputDir + Id;
            console.log("Going to `" + Url + "`");
            await page.goto(Url);
            const Body = await page.$('body');
            let Size = await Body.boundingBox();
            const Width = Size.width;
            const Height = Size.height;

            await page.setViewport({
                width: 1920,
                height: 1080,
                deviceScaleFactor: 2
            })
            await page.screenshot(
                {
                    path: OutputDir + path.dirname(Id) + "/" +
                        path.basename(Id, path.extname(Id)) + ResultExtension,
                    clip: {
                        x: 0, y: 0,
                        width: Width,
                        height: Height
                    },
                    omitBackground: true
                }
            );
            console.log("Processed " + Id);
        }
    }

    await browser.close();
})();
