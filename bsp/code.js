"use strict"

function random(min, max) {
    return Math.floor(Math.random() * (max - min + 1) + min);
}

class Point {
    constructor(x, y) {
        this.x = x;
        this.y = y;
    }
}

class Tree {
    constructor(leaf) {
        this.childs = [];
        this.leaf = leaf;
    }

    add(tree) {
        if (this.childs.length > 2) {
            console.log('This is supposed to be a binary tree!');
            return;
        }
        this.childs.push(tree);
    }

    paint(c, floor) {
        this.leaf.paint(c, floor);
        if (this.childs.length == 0) {
            return;
        }
        for (var i = 0; i < this.childs.length; i++) {
            this.childs[i].paint(c, floor);
        }
    }

    leafs() {
        if (this.childs.length == 0) {
            return [this.leaf];
        }
        var res = [];
        for (var i = 0; i < this.childs.length; i++) {
            var foo = this.childs[i].leafs();
            res = res.concat(foo);
        }
        return res;
    }
}

class FuckRoom {
    constructor(container) {
        var x, y, w, h
        var rand_x = random(1, Math.floor(container.width/3))
        var rand_y = random(1, Math.floor(container.height/3))

        this.x = container.position.x + rand_x;
        this.y = container.position.y + rand_y;
        var diff_x = this.x - container.position.x;
        var diff_y = this.y - container.position.y;

        this.w = container.width - diff_x;
        this.h = container.height - diff_y;
        this.w -= random(0, this.w / 3);
        this.h -= random(0, this.h / 3);

    }

    paint(c, floor) {
        c.fillStyle = "#ABABAB";
        c.fillRect(floor.margin_x + (this.x * floor.square),
                   floor.margin_y + (this.y * floor.square),
                   floor.square * this.w, floor.square * this.h);
    }
}

class Room {
    constructor(width, height, square, options) {
        this.height = height;
        this.width = width;
        this.square = square;
        this.swidth = width * this.square;
        this.sheight = height * this.square;
        this.position = new Point(0, 0);
        this.options = options;
    }

    random_split() {
        // Creates 2 random rooms, splitted from this room
        var res = [];
        var r1, r2;
        var is_horizontal = random(0, 1);
        var ratio = 0.45;
        if (is_horizontal == 1) {
            var h = parseInt(this.height * ratio);
            r1 = new Room(this.width, h, this.square, this.options);
            r1.position = new Point(this.position.x, this.position.y)
            r2 = new Room(this.width, this.height - h, this.square, this.options)
            r2.position = new Point(this.position.x, this.position.y + h)
        } else {
            var w = parseInt(this.width * ratio);
            r1 = new Room(w, this.height, this.square, this.options);
            r1.position = new Point(this.position.x, this.position.y);
            r2 = new Room(this.width - w, this.height, this.square, this.options);
            r2.position = new Point(this.position.x + w, this.position.y);
        }
        console.log(r1, r2);
        res = [r1, r2];
        return res;
    }

    growRooms(steps) {
        var Root = new Tree(this);
        if (steps == 0) {
            return Root;
        }
        var childs = this.random_split();
        for (var i = 0; i < childs.length; i++) {
            Root.add(childs[i].growRooms(steps-1));
        }
        return Root;
    }

    paint(c, floor) {
        c.strokeStyle = '#0F0';
        c.lineWidth = 2;
        c.strokeRect(floor.margin_x + (this.position.x * this.square),
                     floor.margin_y + (this.position.y * this.square),
                     this.width * this.square, this.height * this.square);
    }
}


class Floor extends Room {
    constructor(width, height, canvas, _options) {
        if (_options == undefined)
            _options = {};

        var options = {
            discard: 3,
            steps: 3,
            fullrooms: true,
        }
        jQuery.extend(options, _options);

        var squarex = (1024 - 50) / width;
        var squarey = (500 - 50) / height;
        var square;
        if (squarex * height > 500) {
            square = squarey;
        } else {
            square = squarex;
        }
        super(width, height, square, options);
        this.c = canvas;
        this.margin_x = (1024 - this.swidth) / 2;
        this.margin_y = (500 - this.sheight) / 2;
        this.steps = options['steps'];
        this.room_tree = this.growRooms(this.steps);
        console.log(this.room_tree);
        this.real_rooms = [];
        var leafs = this.room_tree.leafs();
        for (var i = 0; i < leafs.length; i++) {
            this.real_rooms[i] = new FuckRoom(leafs[i]);
        }
        console.log(this.real_rooms);
        this.paint_all(this.c);
        this.export();
    }

    export() {
        var res = Array();
        for (var i = 0; i < this.height + 1; i++) {
            res[i] = Array();
            for (var j = 0; j < this.width + 1; j++) {
                res[i][j] = ' ';
            }
        }

        var leafs = this.room_tree.leafs();
        for (var i = 0; i < leafs.length; i++) {
            var leaf = leafs[i];
            var start_x = leaf.position.x
            var start_y = leaf.position.y
            for (var x = 0; x < leaf.width + 1; x++) {
                res[start_y][start_x + x] = 'a';
                res[start_y + leaf.height][start_x + x] = 'a';
            }
            for (var y = 0; y < leaf.height; y++) {
                res[start_y + y][start_x] = 'a';
                res[start_y + y][start_x + leaf.width] = 'a';
            }
        }

        for (var i = 0; i < this.real_rooms.length; i++) {
            var room = this.real_rooms[i];
            for (var x = room.x; x < room.w + room.x; x++) {
                for (var y = room.y; y < room.h + room.y; y++) {
                    res[y][x] = 'r';
                }
            }
        }
        var out = '';
        for (var i = 0; i < res.length; i++) {
            for (var j = 0; j < res[i].length; j++) {
                out += res[i][j];
            }
            out += '\n';
        }
        $('#raster').html(out);
    }

    paint_all(c) {
        this.paintGrid(c);
        this.room_tree.paint(c, this);
        for (var i = 0; i < this.real_rooms.length; i++) {
            console.log(this.real_rooms[i]);
            this.real_rooms[i].paint(c, this);
        }
    }

    paintGrid(c) {
        c.clearRect(0, 0, 1024, 500);
        //c.fillStyle = '#ABABAB';
        //c.fillRect(this.margin_x, this.margin_y, this.swidth, this.sheight);
        c.beginPath();
        c.strokeStyle = '#ABABAB';
        c.lineWidth = 0.5;
        for (var i = 0; i <= this.width; i++) {
            c.moveTo(this.margin_x + (i * this.square), this.margin_y);
            c.lineTo(this.margin_x + (i * this.square),
                     this.margin_y + this.sheight);
        }
        for (var i = 0; i <= this.height; i++) {
            c.moveTo(this.margin_x, this.margin_y + (i * this.square));
            c.lineTo(this.margin_x + this.swidth,
                     this.margin_y + (i * this.square));
        }
        c.stroke();
        c.closePath();
    }
}


;(function() {
    var get_new_seed = function() {
        var seed = CryptoJS.MD5("" + new Date().getTime()).toString();
        $('input[name=seed]').val(seed);
    }

    var context = $('#viewport')[0].getContext('2d');

    var initFloor = function() {
        $('.error').removeClass('visible');
        var seed = $('input[name=seed]').val();
        var width = $('input[name=width]').val();
        var height = $('input[name=height]').val();
        var steps = $('input[name=steps]').val();
        var discard = $('input[name=discard]').val();
        var fullrooms = $('input[name=fullrooms]').prop('checked');
        Math.seedrandom(seed);
        try {
            var f = new Floor(parseInt(width), parseInt(height), context, {
                steps: parseInt(steps),
                discard: parseInt(discard),
                fullrooms: fullrooms,
            });
        } catch (exception) {
            $('.error').addClass('visible');
            $('.error').text(exception);
        }
    }

    get_new_seed();
    initFloor();

    $('input[name=width]').change(initFloor);
    $('input[name=height]').change(initFloor);
    $('input[name=steps]').change(initFloor);
    $('input[name=discard]').change(initFloor);
    $('input[name=fullrooms]').change(initFloor);
    $('button[name=load]').click(initFloor);
    $('button[name=new_seed]').click(function() {
        get_new_seed();
        initFloor();
    });
}());
