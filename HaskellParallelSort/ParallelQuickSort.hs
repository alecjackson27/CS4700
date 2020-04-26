module Main where

import Prelude
import System.Environment (getArgs)
import Data.List
import Data.Time.Clock
import System.CPUTime
import System.Random
import Control.Parallel
import Control.Exception (evaluate)
import Control.DeepSeq (rnf, deepseq)
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
    let rs = randomlist (len * 10) seed

    printf "Sorting %d elements 10 times...\n" len

    start <- getCurrentTime
    let listOfSortedLists = [quickSort (take len (drop (x * len) rs)) `deepseq` x + x| x <-[0..9]]
    -- Just need to print out the sums of x + x to force quickSort to be evaluated
    -- The operation I used was arbitrary, the purpose was to makes sure the entirety
    -- of the sorted list is evaluated, which is why I used deepseq
    print  [listOfSortedLists !! i |  i <- [0..9]]
    end <- getCurrentTime
    printf "Average Time elapsed: %s\n" (show ((end `diffUTCTime` start) / 10))