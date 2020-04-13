module Main where

import Prelude
import System.Environment (getArgs)
import Data.List
import Data.Time.Clock
import System.CPUTime
import System.Random
import Control.Parallel
import Control.Exception (evaluate)
import Control.DeepSeq (rnf)
import Text.Printf

forceEval :: [a] -> ()
forceEval xs = go xs `pseq` ()
    where go (_:xs) = go xs
          go [] = 1

quickSort [] = []
quickSort (x:xs) = small `par` (big `par` 
    (small ++ [x] ++ big))
    where
        small = quickSort [p | p <-  xs, p <= x]
        big = quickSort [p | p <- xs, p > x]

randomlist :: Int -> StdGen -> [Int]
randomlist n generator = forceEval result `seq` result
                where
                    result = take n (randoms generator)

time = do
    t <-  getCurrentTime
    c <-  getCPUTime
    return (t,c)

measure f p = do
   (t1, c1) <-  time
   evaluate $ rnf $ f p
   (t2, c2) <-  time
   return (diffUTCTime t2 t1, c2 - c1)

main = do
    args <- getArgs
    let len = read (head args)
    seed  <- newStdGen
    let rs = randomlist len seed

    printf "Sorting %d elements...\n" len

    (t, cpu) <-  measure quickSort rs
    printf "CPU Time: %d\nTime elapsed: %s\n" cpu (show t)