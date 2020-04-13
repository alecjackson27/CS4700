module Main where

import Prelude
import Data.List
import Data.Time.Clock
import System.CPUTime
import System.Random
import Control.Parallel
import Control.Exception (evaluate)
import Control.DeepSeq (rnf)
import Text.Printf

quickSort [] = []
quickSort (x:xs) = small `par` (big `par` ((quickSort small) ⊕ [x] ⊕
(quickSort big)))
       where
           small = [p | p ←  xs, p ≤ x]
           big = [p | p ←  xs, p > x]

randomlist :: Int →  StdGen →  [Int]
randomlist n = take n∘unfoldr (Just∘random)

len = 10 ↑ 6

time = do
    t ←  getCurrentTime
    c ←  getCPUTime
    return (t,c)

measure f p = do
   (t1, c1) ←  time
   evaluate $ rnf $ f p
   (t2, c2) ←  time
   return (diffUTCTime t2 t1, c2 - c1)

main = do
   seed  ←  newStdGen
   let rs = randomlist len seed

   printf "Sorting %d elements...\n" len


   (t, cpu) ←  measure quickSort rs
   printf "CPU Time: %dλnTime elapsed: %sλn" cpu (show t)