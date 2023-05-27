import * as Gumshoe from "./gumshoe-patched";
import { createPopper } from '@popperjs/core';

////////////////////////////////////////////////////////////////////////////////
// Scroll Handling
////////////////////////////////////////////////////////////////////////////////
var tocScroll = null;
var isVersionPickerOpened = false;
var header = null;
var lastScrollTop = window.pageYOffset || document.documentElement.scrollTop;
const GO_TO_TOP_OFFSET = 64;

function scrollHandlerForHeader() {
  if (Math.floor(header.getBoundingClientRect().top) == 0) {
    header.classList.add("scrolled");
  } else {
    header.classList.remove("scrolled");
  }
}

function scrollHandlerForBackToTop(positionY) {
  if (positionY < GO_TO_TOP_OFFSET) {
    document.documentElement.classList.remove("show-back-to-top");
  } else {
    if (positionY < lastScrollTop) {
      document.documentElement.classList.add("show-back-to-top");
    } else if (positionY > lastScrollTop) {
      document.documentElement.classList.remove("show-back-to-top");
    }
  }
  lastScrollTop = positionY;
}

function scrollHandlerForTOC(positionY) {
  if (tocScroll === null) {
    return;
  }

  // top of page.
  if (positionY == 0) {
    tocScroll.scrollTo(0, 0);
  } else if (
    // bottom of page.
    Math.ceil(positionY) >=
    Math.floor(document.documentElement.scrollHeight - window.innerHeight)
  ) {
    tocScroll.scrollTo(0, tocScroll.scrollHeight);
  } else {
    // somewhere in the middle.
    const current = document.querySelector(".scroll-current");
    if (current == null) {
      return;
    }

    // https://github.com/pypa/pip/issues/9159 This breaks scroll behaviours.
    // // scroll the currently "active" heading in toc, into view.
    // const rect = current.getBoundingClientRect();
    // if (0 > rect.top) {
    //   current.scrollIntoView(true); // the argument is "alignTop"
    // } else if (rect.bottom > window.innerHeight) {
    //   current.scrollIntoView(false);
    // }
  }
}

function scrollHandler(positionY) {
  scrollHandlerForHeader();
  scrollHandlerForBackToTop(positionY);
  scrollHandlerForTOC(positionY);
}

////////////////////////////////////////////////////////////////////////////////
// Theme Toggle
////////////////////////////////////////////////////////////////////////////////
function setTheme(mode) {
  if (mode !== "light" && mode !== "dark" && mode !== "auto") {
    console.error(`Got invalid theme mode: ${mode}. Resetting to auto.`);
    mode = "auto";
  }

  document.body.dataset.theme = mode;
  localStorage.setItem("theme", mode);
  console.log(`Changed to ${mode} mode.`);
}

function cycleThemeOnce() {
  const currentTheme = localStorage.getItem("theme") || "auto";
  const prefersDark = window.matchMedia("(prefers-color-scheme: dark)").matches;

  if (prefersDark) {
    // Auto (dark) -> Light -> Dark
    if (currentTheme === "auto") {
      setTheme("light");
    } else if (currentTheme == "light") {
      setTheme("dark");
    } else {
      setTheme("auto");
    }
  } else {
    // Auto (light) -> Dark -> Light
    if (currentTheme === "auto") {
      setTheme("dark");
    } else if (currentTheme == "dark") {
      setTheme("light");
    } else {
      setTheme("auto");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Setup
////////////////////////////////////////////////////////////////////////////////
function setupScrollHandler() {
  // Taken from https://developer.mozilla.org/en-US/docs/Web/API/Document/scroll_event
  let last_known_scroll_position = 0;
  let ticking = false;

  window.addEventListener("scroll", function (e) {
    last_known_scroll_position = window.scrollY;

    if (!ticking) {
      window.requestAnimationFrame(function () {
        scrollHandler(last_known_scroll_position);
        ticking = false;
      });

      ticking = true;
    }
  });
  window.scroll();
}

function setupScrollSpy() {
  if (tocScroll === null) {
    return;
  }

  // Scrollspy -- highlight table on contents, based on scroll
  Gumshoe(
      ".toc-tree a", {
        reflow: true,
        recursive: true,
        navClass: "scroll-current",
        offset: () => {
          let rem = parseFloat(getComputedStyle(document.documentElement).fontSize);
          return header.getBoundingClientRect().height + 0.5 * rem + 1;
        },
      }
  );
}

function setupTheme() {
  // Attach event handlers for toggling themes
  const buttons = document.getElementsByClassName("theme-toggle");
  Array.from(buttons).forEach((btn) => {
    btn.addEventListener("click", cycleThemeOnce);
  });
}

function findSectionTitle(section) {
  for (let i = 0; i < section.children.length; ++i) {
    let element = section.children[i];
    let tagName = element.tagName.toLowerCase();

    for (let size = 1; size < 7; ++size) {
      if (tagName === "h" + size.toString()) {
        let text = "";
        element.childNodes.forEach(function (node) {
          if (node.nodeType === Node.TEXT_NODE) {
            text += node.nodeValue;
          }
        })

        if (text.trim() === "") {
          Array.from(element.getElementsByClassName("heading-title"))
              .forEach(function (node) {
            text += node.innerText;
          });
        }

        return text;
      }
    }
  }
}

function addSubsectionsToToc(toc, section) {
  for (let i = 0; i < section.children.length; ++i) {
    let element = section.children[i];

    if (element.tagName.toLowerCase() === "section") {
      let title = findSectionTitle(element);
      console.log(element);
      console.log(title);
      let li = document.createElement("li");
      let link = document.createElement("a");
      link.className = "reference internal";
      link.href = "#" + element.id;
      link.innerHTML = title;
      li.appendChild(link);

      let sublist = document.createElement("ul");
      addSubsectionsToToc(sublist, element);

      if (sublist.children.length > 0) {
        li.appendChild(sublist);
      }

      toc.appendChild(li);
    } else {
      addSubsectionsToToc(toc, element);
    }
  }
}

function generateToc() {
  let article = document.getElementById("main-article");
  let ul = document.createElement("ul");

  addSubsectionsToToc(ul, article);

  document.getElementById("toc-tree").appendChild(ul);
}

let versionPicker;
let versionsList;

let popperInstance;

function show() {
  // Make the tooltip visible
  versionsList.setAttribute('data-show', '');

  // Enable the event listeners
  popperInstance.setOptions((options) => ({
    ...options,
    modifiers: [
      ...options.modifiers,
      { name: 'eventListeners', enabled: true },
    ],
  }));

  // Update its position
  popperInstance.update();

  isVersionPickerOpened = true;
}

function hide() {
  // Hide the tooltip
  versionsList.removeAttribute('data-show');

  // Disable the event listeners
  popperInstance.setOptions((options) => ({
    ...options,
    modifiers: [
      ...options.modifiers,
      { name: 'eventListeners', enabled: false },
    ],
  }));

  isVersionPickerOpened = false;
}

function toggleVersionPicker(event) {
  if (isVersionPickerOpened) {
    hide();
  } else {
    show();
  }

  event.stopPropagation();
}

function setupVersionPicker() {
  versionPicker = document.querySelector('#version-picker-text');

  if (versionPicker) {
    versionsList = document.querySelector('#available-versions');
    popperInstance = createPopper(versionPicker, versionsList, {
      placement: 'bottom-start',
      modifiers: [
        {
          name: 'offset',
          options: {
            offset: [-3, 8],
          },
        },
      ],
    });
    hide();

    versionPicker.addEventListener(
        "click",
        toggleVersionPicker
    );
    document.addEventListener(
        "click",
        hide
    )
  }
}

function setup() {
  setupTheme();
  generateToc();
  setupScrollHandler();
  setupScrollSpy();
  setupVersionPicker();
}

////////////////////////////////////////////////////////////////////////////////
// Main entrypoint
////////////////////////////////////////////////////////////////////////////////
function main() {
  document.body.parentNode.classList.remove("no-js");

  header = document.querySelector("#big-header");
  tocScroll = document.querySelector(".toc-scroll");

  setup();
}

document.addEventListener("DOMContentLoaded", main);
