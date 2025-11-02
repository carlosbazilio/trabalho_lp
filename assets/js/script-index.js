const PALETTE = { //poorly implemented
    empty: 'white',
    correctAnswer: 'blue',
    lastAlocated: 'salmon',
    recentlyOccupied: '#6C3BAA',
    occupied: 'purple',
    toBeRemoved: 'red',
};
let defaultRegisters = [];// {register, color}
let lastAlocated = [];
let randomRegisterAmount = 0;
let registerAddedByInput = []; /// {register, color, name}/

// recognize the command
document.addEventListener("submit", (event) => {

    event.preventDefault();

    const input = document.querySelector(".submiText");
    const oldButton = document.getElementById("mainButton");
    const oldButtonClone = oldButton.cloneNode(true); //

    var texto = input.value.trim().toLowerCase(); //user input text

    let error = false; //shows if nothing went wrong
    // console.log(texto);
    input.value = " ";// resets the textbox


    // // --- Destroy previous random registers ---
    // const randomContainer = document.querySelector(".randomRegister-Container");
    // randomContainer.innerHTML = ""; // remove all children


    const command = texto.trim().split(" ");
    const commandType = command[0]
    if (commandType === "new") {
        error = commandCreate(command, 3);
    }
    else if (commandType === "del" || commandType === "delete") {
        error = commandDelete(command, 2);
    }

    else if (command[1] === "=") {
        const source = registerAddedByInput.find(obj => obj.Nome === command[0]);
        const target = registerAddedByInput.find(obj => obj.Nome === command[2]);
        if (!source) {
            warning(`${source} not found`);
        }
        if (!target) {
            warning(`${target} not found`);
        }
        let { Nome } = source;
        let { indexStart, indexEnd } = target;
        let aux = { Nome, indexStart, indexEnd };

        error = commandDelete(["del", source.Nome], 2); //delete a GAMBIARRA
        registerAddedByInput.push(aux);


    }
    else {
        warning("commandType not recognized");
        error = true;

    }
    if (!error) {
        // --- substitui o botão ---
        const newButton = document.createElement("button");
        newButton.id = "confirmButton";
        newButton.textContent = "Confirm";
        newButton.type = "button"; // importante! evita submeter de novo o form


        // substitui no DOM
        oldButton.replaceWith(newButton);

        input.style.display = "none";
        // adiciona funcionalidade ao novo botão
        newButton.addEventListener("click", () => {
            console.log("Confirmed!");

            let startIndex = -1, endIndex = -1;
            for (let index = 0; index < defaultRegisters.length; index++) {
                const item = defaultRegisters[index];

                if (item.register.style.backgroundColor === PALETTE.correctAnswer) {
                    if (startIndex === -1) startIndex = index;// storing the information of the blue elements
                    endIndex = index;
                    item.color = PALETTE.lastAlocated;
                }
                if (item.register.style.backgroundColor === PALETTE.lastAlocated) {
                    item.color = PALETTE.recentlyOccupied;
                }
                if (item.register.style.backgroundColor === PALETTE.toBeRemoved) {
                    console.log("hello");
                    item.color = PALETTE.empty;
                }
            };
            resetRegisters(defaultRegisters);
            if (startIndex != -1) {
                registerAddedByInput.push({
                    Nome: command[1],
                    indexStart: Number(startIndex),
                    indexEnd: Number(endIndex),
                })
            }
            createHistoryBoxes();


            input.style.display = "inline";// to revert the button
            newButton.replaceWith(oldButtonClone);
            randomRegisterAmount = 0;//random registers amount to 0 after being alocated

        });
    }
    else {//if problem was detected in user input, exit == 1, so we reset to 0
        error = 0;
    }
})
// Main part of the code
document.addEventListener('DOMContentLoaded', () => {

    lastAlocated = [];
    // createRandomRegisters(randomRegisterAmount);
    createDefaultRegisters(defaultRegisters, lastAlocated);



    // --- Define runAlgorithm inside same scope ---
    allocationStrategy(randomRegisterAmount);

    attachRadioListeners();
});

//----- Utility ------

function groups(array) {
    const groups = {};

    array.forEach(item => {
        const key = item.indexStart;
        if (!groups[key]) {
            groups[key] = [];
        }
        groups[key].push(item);
    });

    return groups;
}


// [{Nome: "A", ...}, {Nome: "C", ...}]
function getRandomRGBA(alpha = 1) {
    const r = getRandomInt(0, 255);
    const g = getRandomInt(0, 255);
    const b = getRandomInt(0, 255);
    return `rgba(${r}, ${g}, ${b}, ${alpha})`;
}
function getRandomInt(min, max) {
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

function sleep(seconds) {
    return new Promise(resolve => {
        setTimeout(resolve, seconds * 1000);
    })
}
function resetRegisters(defaultRegisters) { // reset their original color
    defaultRegisters.forEach(item => {
        item.register.style.backgroundColor = item.color; // back to base color
    });
}
//----- Starting Up Registers ------
function createRandomRegisters(randomRegisterAmount) {
    const randomRegisterContainer = document.querySelector('.randomRegister-Container');

    // --- Create  random registers ---
    for (let i = 0; i < randomRegisterAmount; i++) {
        const randomRegister = document.createElement('div');
        randomRegister.className = 'randomRegister';
        randomRegister.style.backgroundColor = PALETTE.occupied;
        randomRegisterContainer.appendChild(randomRegister);
    }
}
function createDefaultRegisters(defaultRegisters, lastAlocated) {

    const container = document.querySelector('.register-container');
    let registerAmount = 30;
    let randomness = 0.3;
    if (isMobile()) {
        registerAmount = 20;
        randomness = 0.4

    }

    // --- Create static registers ---
    let isLastAlocated = false;
    let isFirstOfRow = true;
    let isFirstRegisterCreated = true;

    for (let i = 0; i < registerAmount; i++) {
        const HTMLregister = document.createElement('div');
        let isRegisterOccupied = Math.random() < randomness; //random odds of being painted "occupied"
        HTMLregister.className = 'register';


        //colloring Logic
        if (isRegisterOccupied) {
            if (isLastAlocated) {
                lastAlocated.push(HTMLregister);
                //odds of being pushed in the "lastAlocated" queue
            } else if (isFirstRegisterCreated || (isFirstOfRow && getRandomInt(1, 10) < 2)) {
                lastAlocated.length = 0;//to empty the vector
                lastAlocated.push(HTMLregister);

                isLastAlocated = true;
                isFirstOfRow = false;
            } else {
                isFirstOfRow = false;
                isLastAlocated = false;
            }
            defaultRegisters.push({ register: HTMLregister, color: PALETTE.occupied });
            isFirstRegisterCreated = false;
        } else {//break the sequence, preparing to  the next
            isFirstOfRow = true;
            isLastAlocated = false;
            defaultRegisters.push({ register: HTMLregister, color: PALETTE.empty });
        }

        container.appendChild(HTMLregister);
    }
    //if register exist in dR and lA
    defaultRegisters.forEach(item => {
        if (lastAlocated.some(last => last === item.register)) {
            item.color = PALETTE.lastAlocated;
        }
        item.register.style.backgroundColor = item.color;
    });

}
function createHistoryBoxes() {

    const history = document.querySelector((".historyContainer"));
    history.innerHTML = "";

    //atualizar historico
    registerAddedByInput.forEach(element => {
        let quantityOfRegisters = element.indexEnd - element.indexStart + 1;

        const historyElement = document.createElement("div");
        historyElement.className = "history";
        historyElement.className = "box";
        historyElement.textContent = `${element.Nome} qnt:${quantityOfRegisters}`;


        // Highlight duplicates
        historyElement.addEventListener("mouseover", () => {
            for (let i = element.indexStart; i <= element.indexEnd; i++) {
                defaultRegisters[i].register.style.backgroundColor = 'pink';

            }
        });

        historyElement.addEventListener("mouseout", () => {
            resetRegisters(defaultRegisters);
        });

        history.appendChild(historyElement);
    })


}
function isMobile() {
    return window.innerWidth <= 768; // adjust breakpoint as needed
}

console.log(isMobile());
//----------- Allocation algorithms ----------------


// will to the white registers
function allocationStrategy(randomRegisterAmount) {
    const selected = document.querySelector('input[name="checked"]:checked').value;
    console.log("hello");
    console.log(selected);
    let foundMatch = false;
    switch (selected) {
        case 'first':
            resetRegisters(defaultRegisters);
            foundMatch = firstFit(defaultRegisters, randomRegisterAmount);
            break;
        case 'best':
            resetRegisters(defaultRegisters);
            foundMatch = bestFit(defaultRegisters, randomRegisterAmount);
            break;
        case 'worst':
            resetRegisters(defaultRegisters);
            foundMatch = worstFit(defaultRegisters, randomRegisterAmount);
            break;
        case 'next':
            resetRegisters(defaultRegisters);
            foundMatch = nextFit(defaultRegisters, randomRegisterAmount);
            break;
    }


    if (!foundMatch && randomRegisterAmount != 0) {
        warning("No available space! Please delete something.");
    }
}
function firstFit(defaultRegisters, randomRegisterAmount) {
    let currentBlock = [];
    let foundMatch = false;

    for (let index = 0; index < defaultRegisters.length; index++) {
        const item = defaultRegisters[index];

        if (foundMatch) break;

        if (item.color === PALETTE.empty) {
            currentBlock.push(item.register);

            if (currentBlock.length === randomRegisterAmount) {
                currentBlock.forEach(blankSpace => {
                    blankSpace.style.backgroundColor = PALETTE.correctAnswer;//styles without changing proprierty
                });
                foundMatch = true;
                break;
            }
        } else {
            currentBlock = [];
        }
    }

    if (!foundMatch) {
        // console.log("nao ha lugar valido");
    }
    return foundMatch;
}
function nextFit(defaultRegisters, randomRegisterAmount) {
    let newArray = defaultRegisters;// to reenterpret the start of the list
    let lastAlocatedIndex = -1;

    // Find the (PALETTE.lastAlocated) register
    for (let i = 0; i < defaultRegisters.length; i++) {
        if (defaultRegisters[i].register.style.backgroundColor === PALETTE.lastAlocated) {
            lastAlocatedIndex = i;
            break;
        }
    }

    if (lastAlocatedIndex === -1) {
        console.log("Nenhum registro do ultimo registrador encontrado, usando firstFit normalmente");
        return firstFit(defaultRegisters, randomRegisterAmount);
    }

    // Build array starting after PALETTE.lastAlocated
    for (let i = lastAlocatedIndex; i < defaultRegisters.length; i++) {
        const register = defaultRegisters[i];
        if (register.register.style.backgroundColor === PALETTE.empty) {
            const firstHalf = defaultRegisters.slice(i);
            const secondHalf = defaultRegisters.slice(0, i);

            newArray = firstHalf.concat(secondHalf);
            break;
        }
    }

    return firstFit(newArray, randomRegisterAmount);
}
function bestFit(defaultRegisters, randomRegisterAmount) {
    let currentBlock = [];// bloco com a sequencia de registradores livres atual
    let allBlankBlocks = []; // vetor  de blacos
    let exactMatch = null;
    let bestBlock;//lista de registradores, do bloco mais apropiado

    for (let index = 0; index < defaultRegisters.length; index++) {
        const item = defaultRegisters[index];

        if (item.color === PALETTE.empty) {
            currentBlock.push(item);

        } else {// adds to validBlocks when it breaks

            if (currentBlock.length === randomRegisterAmount) {// best case, quit
                exactMatch = currentBlock;
                break;
            }
            allBlankBlocks.push(currentBlock);
            currentBlock = [];

        }
    }

    //adiciona ultimo elemento, se o 
    if (!exactMatch && currentBlock.length > 0) {
        if (currentBlock.length === randomRegisterAmount) {
            exactMatch = currentBlock;
        } else {
            allBlankBlocks.push(currentBlock);
        }
    }

    if (exactMatch) {
        bestBlock = exactMatch;
        console.log("Match exato encontrado!");
    }
    else {

        let validBlocks = allBlankBlocks.filter(block => block.length >= randomRegisterAmount);// o bloco deve ser maior ou igual a qnt dados alocados 
        if (validBlocks.length === 0) {
            console.log("nao ha lugar valido");
            return false;
        }
        // Find the smallest valid block (Best Fit)
        bestBlock = validBlocks[0];
        validBlocks.forEach(block => {
            if (block.length <= bestBlock.length) { //BEST FIT strategy
                bestBlock = block;
            }
        });
        // Allocate in the best fit block
    }

    for (let i = 0; i < randomRegisterAmount; i++) {
        bestBlock[i].register.style.backgroundColor = PALETTE.correctAnswer;
    }
    console.log(`Allocated ${randomRegisterAmount} registers in block of size ${bestBlock.length} `);
    return true;
}
function worstFit(defaultRegisters, randomRegisterAmount) {
    let currentBlock = [];
    let allBlankBlocks = []; // vector of  possible solutions
    let exactMatch = null;
    let worseBlock = null;

    for (let index = 0; index < defaultRegisters.length; index++) {
        const item = defaultRegisters[index];

        if (item.color === PALETTE.empty) {
            currentBlock.push(item);

        } else {// adds to validBlocks when it breaks

            if (currentBlock.length === randomRegisterAmount) {// best case, quit
                exactMatch = currentBlock;
                break;
            }
            allBlankBlocks.push(currentBlock);
            currentBlock = [];

        }
    }

    //adiciona ultimo elemento, se o 
    if (!exactMatch && currentBlock.length > 0) {
        if (currentBlock.length === randomRegisterAmount) {
            exactMatch = currentBlock;
        } else {
            allBlankBlocks.push(currentBlock);
        }
    }

    if (exactMatch) {
        worseBlock = exactMatch;
        console.log("Match exato encontrado!");
    }
    else {

        let validBlocks = allBlankBlocks.filter(block => block.length >= randomRegisterAmount);// o bloco deve ser maior ou igual a qnt dados alocados 
        if (validBlocks.length === 0) {
            console.log("nao ha lugar valido");
            return false;
        }
        // Find the smallest valid block (Best Fit)
        worseBlock = validBlocks[0];
        validBlocks.forEach(block => {
            if (block.length >= worseBlock.length) { //WORSE FIT strategy
                worseBlock = block;
            }
        });
        // Allocate in the best fit block
    }

    for (let i = 0; i < randomRegisterAmount; i++) {
        worseBlock[i].register.style.backgroundColor = PALETTE.correctAnswer;
    }
    console.log(`Allocated ${randomRegisterAmount} registers in block of size ${worseBlock.length} `);
    return true;
}

function attachRadioListeners() {
    document.querySelectorAll('input[name="checked"]').forEach(radio => {
        radio.addEventListener('change', async () => {
            resetRegisters(defaultRegisters);
            await sleep(0.15);
            allocationStrategy(randomRegisterAmount);
        });
    });
}
function warning(warningText) {
    const warning = document.getElementById("warning");
    warning.textContent = warningText;
    warning.style.display = "block";
    setTimeout(() => warning.style.display = "none", 3000);

    defaultRegisters.forEach(item => {
        if (item.register.style.backgroundColor === PALETTE.empty) {
            item.register.style.backgroundColor = "pink";
        }
    });
    resetRegisters(defaultRegisters);
}


function commandCreate(command, requiredNumberCommands) {
    const numberCommands = command.length;
    const name = command[1];

    const alreadyExists = registerAddedByInput.find(obj => obj.Nome === name);//looks in the registerAddedByInput
    if (alreadyExists) {
        warning("sorry, this name is already in use, chose another");
        error = true;
        return true
    }
    if (numberCommands != requiredNumberCommands) {
        warning(numberCommands < requiredNumberCommands ? "too few" : "too many", "commands");
        error = true;
        return error
    }
    const quantityOfRegisters = parseInt(command[2]);
    // createRandomRegisters(quantityOfRegisters);

    // Update global variable if needed
    randomRegisterAmount = quantityOfRegisters;

    // Now run the allocation algorithm
    allocationStrategy(quantityOfRegisters);
    attachRadioListeners();

}

function commandDelete(command, requiredNumberCommands) {

    const numberCommandElements = command.length;
    const name = command[1];

    const alreadyExists = registerAddedByInput.find(obj => obj.Nome === name);//looks in the registerAddedByInput
    if (typeof requiredNumberCommands !== 'number') {
        warning("requiredNumberCommands must be a number");
        return true;
    }
    else if (!alreadyExists) {
        warning("sorry, unable to find");
        return true;
    }
    else if (numberCommandElements != requiredNumberCommands) {
        warning(numberCommandElements < requiredNumberCommands ? "too few" : "too many", "commands");
        return true;
    }

    const registerToBeDeleted = alreadyExists;
    for (let index = registerToBeDeleted.indexStart; index <= registerToBeDeleted.indexEnd; index++) {
        const item = defaultRegisters[index];
        item.register.style.backgroundColor = PALETTE.toBeRemoved;// dont remove it just yet
        item.color = PALETTE.toBeRemoved;
    }
    indexFirstRegister = registerToBeDeleted.indexStart
    registerAddedByInput = registerAddedByInput.filter(obj => obj.indexStart !== indexFirstRegister);//removes all the references to this space
    return false

}

