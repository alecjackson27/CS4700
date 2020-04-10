module Main where

import System.Environment (getArgs)
import System.Random (StdGen, getStdGen, randoms)
import Data.Time.Clock (diffUTCTime, getCurrentTime)
import Control.Parallel (par, pseq)
import GlobalSum

-- Function to force entire generated list to be evaluated immediately, rather than
-- through lazy evaluation. This will allow the main script to only measure the algorithm
-- time, rather than the time to evaluate the list and run the algorithm.
forceEval :: [a] -> ()
forceEval xs = go xs `pseq` ()
    where go (_:xs) = go xs
          go [] = 1

-- Function to generate random numbers to be summed
genRands :: Int -> StdGen -> [Int]
genRands n generator = forceEval result `seq` result
                where
                    result = take n (randoms generator)

-- main script testing the function's runtime.
main = do
    args <- getArgs
    let count = read (head args)
    generator <- getStdGen
    let lst = genRands count generator
    putStrLn $ "Computing global sum of " ++ show count ++ " numbers..."
    start <- getCurrentTime
    let summation = parGlobalSum lst
    putStrLn $ "Sum is " ++ show (summation)
    end <- getCurrentTime
    putStrLn $ "Computation took " ++ show (end `diffUTCTime` start) ++ " seconds."
