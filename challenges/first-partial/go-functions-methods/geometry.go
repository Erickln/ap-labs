// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/
// See page 156.
// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
	"time"
)

type Point struct {
	x float64
	y float64
}
type Line struct {
	A Point
	B Point
}

type Lines []Line
type Path []Point

func (p Point) X() float64 { return p.x }
func (p Point) Y() float64 { return p.y }

func Distance(p, q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.X()-p.X(), q.Y()-p.Y())
}

// Distance returns the distance traveled along the path.
func (path Path) pathDistance() float64 {
	sum := 0.0
	for i := 1; i < len(path); i++ {
		sum += path[i-1].Distance(path[i])
		//println("%0.2f", sum)
	}
	sum += path[0].Distance(path[len(path)-1])
	return sum
}

//func initializePolygon(points []Point) []Line {
func initializePolygon(sides int) ([]Line, Path) {
	var p Path
	rand.Seed(time.Now().UnixNano())
	for i := 0; i < sides; i++ {
		x := rand.Float64() * 100
		y := rand.Float64() * 100
		p = append(p, Point{float64(x), float64(y)})
	}
	len := int(len(p))
	var lines []Line
	var myLine Line
	for i := 0; i < len-1; i++ {
		myLine = Line{p[i], p[i+1]}
		lines = append(lines, myLine)
	}
	myLine = Line{p[len-1], p[0]}
	lines = append(lines, myLine)
	return lines, p
}

func onSegment(a Point, b Point, p Point) bool {
	l1 := Line{a, b}
	if p.X() <= math.Max(l1.A.X(), l1.B.X()) && p.X() <= math.Min(l1.A.X(), l1.B.X()) && (p.Y() <= math.Max(l1.A.Y(), l1.B.Y()) && p.Y() <= math.Min(l1.A.Y(), l1.B.Y())) {
		return true
	}
	return false
}

func orientation(a Point, b Point, c Point) int {
	val := (b.Y()-a.Y())*(c.X()-b.X()) - (b.X()-a.X())*(c.Y()-b.Y())
	if val == 0 {
		return 0 //colinear
	} else if val < 0 {
		return 2 //anti-clockwise direction
	} else {
		return 1 //clockwise direction
	}
}

func doIntersect(l1 Line, l2 Line) bool {
	dir1 := orientation(l1.A, l1.B, l2.A)
	dir2 := orientation(l1.A, l1.B, l2.B)
	dir3 := orientation(l2.A, l2.B, l1.A)
	dir4 := orientation(l2.A, l2.B, l1.B)

	if dir1 != dir2 && dir3 != dir4 && l1.B != l2.A && l1.A != l2.B {
		return true
	}

	if dir1 == 0 && onSegment(l1.A, l1.B, l2.A) {
		return true
	}

	if dir2 == 0 && onSegment(l1.A, l1.B, l2.B) {
		return true
	}

	if dir3 == 0 && onSegment(l2.A, l2.B, l1.A) {
		return true
	}

	if dir4 == 0 && onSegment(l2.A, l2.B, l1.B) {
		return true
	}
	//If none of the cases
	return false
}

func searchInterceptions(lines []Line) bool {
	len := len(lines)
	for i := 0; i < len; i++ {
		for j := 0; j < len; j++ {
			if doIntersect(lines[i], lines[j]) == true {
				return true
			}
		}
	}
	return false
}

func main() {

	if len(os.Args) < 2 {
		fmt.Println("Not enough data")
		return
	}
	sides, _ := strconv.Atoi(os.Args[1]) //Geting number of sides for the polygon
	if sides < 3 {
		fmt.Println("Not enough data, insert at least 3 sides")
		return
	}
	fmt.Printf("- Generating a [%d] sides figure\n", sides)
	lines, points := initializePolygon(sides)
	var inter bool = true
	inter = searchInterceptions(lines)

	//Defines lines with no intersection
	for inter == true {
		lines, points = initializePolygon(sides)
		inter = searchInterceptions(lines)
	}
	result := points.pathDistance()
	fmt.Println("- Figure's vertices")

	for i := range points {
		fmt.Printf(" - (%3.0f,%3.0f)\n", points[i].X(), points[i].Y())
	}
	fmt.Printf("- Figure's Perimeter\n")
	fmt.Print("- ")
	for j := 0; j < len(lines)-1; j++ {
		fmt.Printf("%0.2f + ", Distance(lines[j].A, lines[j].B))
	}
	fmt.Printf("%0.2f = %0.2f\n", Distance(lines[len(lines)-1].A, lines[len(lines)-1].B), result)
}
