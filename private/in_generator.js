const fs = require('node:fs');
const seedrandom = require('seedrandom')
seedrandom("nfsjkdfndivjovdxcv2r3fswlppro", {global: true})

function randomInt(range) {
    return Math.floor(Math.random() * range);
}

function save(type, test_index, content) {    
    fs.writeFile(`../tests/${type}/${type}${test_index}.in`, content, { flag: 'w+' }, err => {
        if (err) {
            console.error(err);
        }
    });
}

function createNumber(maxDepth, maxRange) {
    let number = ''

    let i = 0; let depth = 0; let n = randomInt(maxRange)

    while (i < n) {
        if (depth == 0 || (depth < maxDepth && Math.random() > 0.6)) {
            depth++;
            number += '1'
        } else {
            depth--;
            number += '0'
        }
        i++
    }

    while (depth > 0) {
        number+='0'
        depth--;
    }

    return number + '0'
}

function chooseOperation(stackDepth) {
    if (stackDepth == 0) return 'number'

    let odds = Math.random()

    if (stackDepth == 1) {
        if (odds > 0.35) return 'number'

        if (odds > 0.25) return ':'
        if (odds > 0.2) return '^'
        return '.'
    } else {
        if (odds > 0.5) return 'number'

        if (odds > 0.4) return '+'
        if (odds > 0.3) return '*'
        if (odds > 0.25) return '^'
        if (odds > 0.15) return ':'
        return '.'
    }
}

function makeTests(type, testsNumber, rows, maxDepth, maxLength) {
    for (let testIndex = 0; testIndex < testsNumber; testIndex++) {
        let content = '';
        let stackDepth = 0

        for (let i = 0; i < rows; i++) {
            let operation = chooseOperation(stackDepth)

            if (operation == 'number') {
                content += createNumber(maxDepth, maxLength)
            } else {
                content += operation
            }
            content += '\n'

            if (operation == 'number' || operation == ':') stackDepth++;
            if (operation == '+' || operation == '*' || operation == '.') stackDepth--;
        }

        save(type, testIndex, content)
    }


}

makeTests('small', 1000, 100, 15, 50)
makeTests('medium', 500, 150, 30, 100)
makeTests('large', 400, 300, 50, 150)
