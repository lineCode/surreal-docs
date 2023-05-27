/*
 * Copyright © 2022, Medelfor, Limited. All rights reserved.
 * For any additional information refer to https://medelfor.com
 */

let project_name = 'udocs_processor';
let resources = 'res';
let destination = 'res_out';
let auto_res_prefix = 'dist';

let paths = {
    html: [
        {
            source: ['**/*.html'],
            minify: false
        }
    ],
    scripts: [
        {
            source: 'theme/theme1/theme1.js',
            lang: "ts"
        }
    ],
    styles: [
        {
            bundle_name: 'theme1.css',
            source: [
                'theme/theme1/theme1.sass'
            ],
            processor: "sass"
        },
        {
            bundle_name: 'theme1-extensions.css',
            source: [
                'theme/theme1/theme1-extensions.sass'
            ],
            processor: "sass"
        },
        {
            bundle_name: 'Type.css',
            source: ['types/Type.css']
        },
        {
            bundle_name: 'TypeColors.css',
            source: ['TypeColors.css']
        },
        {
            bundle_name: 'DataInputs.css',
            source: ['functions/DataInputs.css']
        },
        {
            bundle_name: 'Function.css',
            source: ['functions/Function.css']
        }
    ]
};

// do not touch

let gulp = require('gulp')
let browserify = require('browserify');
let source = require('vinyl-source-stream');
let argv = require('yargs').argv;
let ms = require('merge-stream');
let rename = require('gulp-rename');
let tsify = require('tsify');
let uglify = require('gulp-uglify');
let sourcemaps = require('gulp-sourcemaps');
let buffer = require('vinyl-buffer');
let csso = require('gulp-csso');
let concat = require('gulp-concat');
let htmlmin = require('gulp-htmlmin');
let eslint = require('gulp-eslint');
let sass = require('gulp-sass')(require('sass'));
let colors = require('colors');
let fs = require('fs');
let del = require('del');
let util = require('util');
let node_path = require('path');
let clean_css = require('gulp-clean-css');
let gulpif = require('gulp-if');
let path = require('path');

const { series, parallel, watch } = require('gulp');

const scripts = 'scripts';
const misc = 'misc';
const styles = 'styles';
const html = 'html';

const scripts_postfix = scripts + '/' + project_name;
const styles_postfix = styles + '/' + project_name;
const html_postfix = html + '/' + project_name;

const input_scripts = resources + '/' + scripts_postfix;
const input_scripts_project_file = input_scripts + '/tsconfig.json';
const input_misc = resources + '/' + misc;
const input_styles = resources + '/' + styles_postfix;
const input_html = resources + '/' + html_postfix;

if (argv.destination !== undefined) {
    destination = argv.destination;
}

if (argv.auto_res_prefix !== undefined) {
    auto_res_prefix = argv.auto_res_prefix;
}

let res = {
    external_: function(path)
    {
        return auto_res_prefix + '/external_' + path;
    },

    merged_: function(path) {
        return auto_res_prefix + '/merged_' + path;
    },

    dev: function(path) {
        return path + '/dev';
    },

    bundled: function(path) {
        return path + '/bundled';
    }
};

const output_scripts = destination + '/' + input_scripts;
const output_misc = destination + '/' + input_misc;
const output_styles = destination + '/' + input_styles;
const output_html = destination + '/' + input_html;

let prefix = function (prefix, list) {
    for (let i = 0; i < list.length; ++i) {
        list[i] = prefix + list[i];
    }
}

for (let i = 0; i < paths.styles.length; ++i) {
    prefix(res.merged_(input_styles) + '/', paths.styles[i].source);
}
for (let i = 0; i < paths.html.length; ++i) {
    prefix(res.merged_(input_html) + '/', paths.html[i].source);
}

gulp.task('compile-scripts', function () {
    let ts_tasks = paths.scripts.map(function(entry) {
        let src = entry.source;
        let lang = entry.lang;
        let start;
        if (lang === "ts") {
            return browserify({
                entries: [src],
                basedir: res.merged_(input_scripts),
                paths: [
                    '../'
                ]
            })
            .plugin(tsify, {
                project: res.merged_(input_scripts_project_file)
            })
            .bundle()
            .pipe(source(res.merged_(input_scripts) + '/' + src,
                res.merged_(input_scripts)))
            .pipe(rename( function (path) {
                    path.dirname = resources
                        + path.dirname.substr(res.merged_(resources).length);
                    path.extname = '.bundle.js';
                }
            ))
            .pipe(buffer())
            .pipe(sourcemaps.init({loadMaps: true}))
            .pipe(uglify(
                {
                    "keep_fnames": true
                }
            ))
            .pipe(sourcemaps.write('./'))
            .pipe(gulp.dest(destination));
        } else {
            return browserify({
                entries: [src],
                basedir: res.merged_(input_scripts),
                paths: [
                    '../'
                ]
            })
            .bundle()
            .pipe(source(res.merged_(input_scripts) + '/' + src,
                res.merged_(input_scripts)))
            .pipe(rename( function (path) {
                    path.dirname = resources
                        + path.dirname.substr(res.merged_(resources).length);
                    path.extname = '.bundle.js';
                }
            ))
            .pipe(buffer())
            .pipe(sourcemaps.init({loadMaps: true}))
            .pipe(uglify(
                {
                    "keep_fnames": true
                }
            ))
            .pipe(sourcemaps.write('./'))
            .pipe(gulp.dest(destination));
        }
    });

    let merged = ms();
    merged.add(ts_tasks);

    return merged;
});

gulp.task('lint-scripts', function () {
    return gulp
        .src([
            res.merged_(input_scripts) + '/**/*.ts',
            res.merged_(input_scripts) + '/**/*.tsx'
        ], {dot: true})
        .pipe(eslint({
            'overrideConfig': res.merged_(input_scripts) + '/.eslintrc.json',
            'baseConfig': {
                "parserOptions": {
                    "project": [res.merged_(input_scripts_project_file)]
                },
            }
        }))
        .pipe(eslint.failOnError())
        .pipe(eslint.format());
});

gulp.task('process-html', function () {
    let html_tasks = paths.html.map(function(entry) {
        return gulp.src(entry.source, {dot: true})
            .pipe(gulpif(entry.minify, htmlmin({
                collapseWhitespace: true,
                removeComments: true,
                removeCommentsFromCDATA: true,
                collapseBooleanAttributes: true
            })))
            .pipe(gulp.dest(output_html));
    });

    let merged = ms();
    merged.add(html_tasks);

    return merged;
});

gulp.task('process-styles', function () {
    let css_tasks = paths.styles.map(function(entry) {
        if (entry.processor === "sass") {
            return gulp.src(entry.source, {dot: true})
                .pipe(sass({outputStyle: 'compressed'}).on('error', sass.logError))
                .pipe(gulp.dest(output_styles));
        } else {
            return gulp.src(entry.source, {dot: true})
                .pipe(concat(entry.bundle_name))
                .pipe(clean_css())
                .pipe(gulp.dest(output_styles));
        }
    });

    let merged = ms();
    merged.add(css_tasks);

    return merged;
});

gulp.task('process-misc', function () {
    return gulp.src(res.merged_(input_misc) + '/**/*', {dot: true})
        .pipe(gulp.dest(output_misc));
});

gulp.task('clean-merged-res', function (done) {
    del.sync([res.merged_(resources) + '/**/*']);
    done();
});

gulp.task('copy-external-dev-res-to-merged', function () {
    return gulp.src(res.dev(res.external_(resources)) + '/**/*', {dot: true})
        .pipe(gulp.dest(res.merged_(resources)));
});

gulp.task('copy-res-to-merged', function () {
    return gulp.src(resources + '/**/*', {dot: true})
        .pipe(gulp.dest(res.merged_(resources)));
});

gulp.task('copy-external-bundled-res-to-dest', function () {
    return gulp.src(res.bundled(res.external_(resources)) + '/**/*', {dot: true})
        .pipe(gulp.dest(destination + '/' + resources));
});

gulp.task('clean-symlinks',
    function (done) {
        let res_scripts = resources + '/' + scripts;

        console.log('Cleaning symlinks');
        let deleted = 0;

        for (let file of fs.readdirSync(res_scripts)) {
            let full_path = res_scripts + '/' + file;

            if (fs.lstatSync(full_path).isSymbolicLink()) {
                fs.unlinkSync(full_path);
                ++deleted;
            }
        }

        console.log(colors.green('Cleaned ' + deleted + ' symlinks'));

        done();
    }
);

gulp.task('create-symlinks', function (done) {
    let external_res = res.dev(res.external_(resources)) + '/' + scripts;
    let res_scripts = resources + '/' + scripts;

    fs.readdir(external_res,
        function (err, files) {
            if (err) {
                console.log(colors.yellow('Unable to scan external_res' +
                    ' directory or no directory was found: ' + err));
                console.log(colors.yellow('No symlinks will be setup'))
            } else {
                for (let file of files) {
                    let full_path = external_res + '/' + file;

                    if (fs.lstatSync(full_path).isDirectory()) {
                        let path = res_scripts + '/' + file;

                        fs.symlinkSync(node_path.resolve(full_path), node_path.resolve(path), 'junction');

                        console.log(colors.green('Created symlink to ' + file));
                    }
                }
            }

            done();
        });
});

gulp.task('setup-symlinks',
    series(
        'clean-symlinks',
        'create-symlinks'
    )
);

gulp.task('setup-merged-res',
    series(
        'clean-merged-res',
        parallel(
            'copy-external-dev-res-to-merged',
            'copy-res-to-merged'
        ),
        'setup-symlinks'
    )
);

gulp.task('process-scripts',
    parallel(
        'compile-scripts',
        'lint-scripts'
    )
);

gulp.task('clean-dest-res', function (done) {
    del.sync([output_misc + '/**/*']);
    done();
});

gulp.task('clean-styles-res', function (done) {
    del.sync([output_styles + '/**/*']);
    done();
});

gulp.task('clean-misc-res', function (done) {
    del.sync([output_misc + '/**/*']);
    done();
});

gulp.task('clean-html-res', function (done) {
    del.sync([output_html + '/**/*']);
    done();
});

gulp.task('clean-scripts-res', function (done) {
    del.sync([output_scripts + '/**/*']);
    done();
});

gulp.task('init-res',
    parallel(
        'setup-merged-res',
        'clean-dest-res'
    )
);

gulp.task('default-watch', function () {
    watch('.eslintrc.json', { delay: 3000 }, parallel('default'));
    watch('package.json', { delay: 3000 }, parallel('default'));

    let process = function (clean_task, task) {
        return series(
            'setup-merged-res',
            clean_task,
            task
        );
    };

    let wildcard = function (part) {
        return resources + '/' + part + '/**/*';
    };

    watch(wildcard(misc), { delay: 3000 },
        process('clean-misc-res', 'process-misc')
    );
    watch(wildcard(html), { delay: 3000 },
        process('clean-html-res', 'process-html')
    );
    watch(wildcard(scripts), { delay: 3000 },
        process('clean-scripts-res', 'process-scripts')
    );
    watch(wildcard(styles), { delay: 3000 },
        process('clean-styles-res', 'process-styles')
    );

    // copy ext. res to dest. the only issue here is when some of the ext. bundled res
    // were deleted they will stay at dest
    watch(res.external_(resources) + '/**/*', { delay: 3000 },
        parallel(
            'setup-merged-res',
            'copy-external-bundled-res-to-dest'
        )
    );
});

gulp.task('default',
    series(
        'init-res',
        parallel(
            'process-scripts',
            'process-html',
            'process-styles',
            'process-misc',
            'copy-external-bundled-res-to-dest'
        )
    )
);
