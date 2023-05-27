/*
 * Copyright © 2022, Kommpute Entertainment™ Sp. z o.o. All rights reserved.
 * For any additional information refer to https://kommpute.com
 */

/** @type {import('ts-jest').InitialOptionsTsJest} */
module.exports = {
    preset: 'ts-jest',
    moduleNameMapper: {
        '^theme/theme1\/(.*)$': '<rootDir>/theme/theme1/$1'
    }
}
