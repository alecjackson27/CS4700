module Main where

import Data.Time.Clock (diffUTCTime, getCurrentTime)
import System.Environment (getArgs)
import System.Random (StdGen, getStdGen, randoms)
import Control.Parallel (par, pseq)

import GlobalSum

-- testFunction = globalSum
testFunction = parGlobalSum

force :: [a] -> ()
force xs = go xs `pseq` ()
    where go (_:xs) = go xs
          go [] = 1

randomInts :: Int -> StdGen -> [Int]
randomInts k g = let result = take k (randoms g)
                 in force result `seq` result

main = do
  args <- getArgs
  let count | null args = 50000
            | otherwise = read (head args)
  input <- randomInts count `fmap` getStdGen
  putStrLn $ "We have " ++ show (length input) ++ " elements to sum."
  start <- getCurrentTime
  let summation = testFunction input
  putStrLn $ "Sum is " ++ show (summation)
  end <- getCurrentTime
  putStrLn $ show (end `diffUTCTime` start) ++ " elapsed."
